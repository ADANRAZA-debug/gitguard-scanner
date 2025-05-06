#!/usr/bin/env python3
"""
GitGuard Ultimate Recovery Scanner - Professional Secret Recovery Tool

Core Concept:
- Specializes in recovering deleted repositories, folders, and files from Git history
- Finds leaks that developers thought were deleted but are still recoverable
- Comprehensive reporting of all exposed secrets found in recovered content
"""

import os
import re
import json
import shutil
import requests
import argparse
import subprocess
from pathlib import Path
from datetime import datetime
from typing import List, Dict, Optional, Set

# ====================== CONFIGURATION ======================
CONFIG = {
    "TEMP_DIR": "scan_temp",
    "REPORT_DIR": "reports",
    "MAX_COMMITS_TO_SCAN": 2000,  # Increased for thorough history scanning
    "CLEANUP_EMPTY_REPOS": True,
    "GIT_OPERATION_TIMEOUT": 900,  # 15 minutes for git operations
    "RECOVERY_METHODS": {
        "DELETED_FILES": True,
        "DANGLING_OBJECTS": True,
        "STASHES": True,
        "LOST_COMMITS": True,
        "RESET_BRANCHES": True
    }
}

# ====================== RECOVERY PATTERNS ======================
RECOVERY_PATTERNS = {
    # Cloud Secrets
    "AWS_CREDENTIALS": {
        "pattern": r"(aws_access_key_id|aws_secret_access_key)\s*=\s*([A-Za-z0-9+/=]{20,40})",
        "severity": "critical"
    },
    
    # Database Credentials
    "DATABASE_URL": {
        "pattern": r"(postgresql|mysql|mongodb)://[^:]+:[^@]+@[^/]+/[^\s'\"]+",
        "severity": "critical"
    },
    
    # API Keys
    "API_KEY": {
        "pattern": r"(?i)(api|access)[_-]?key[_-]?\s*[:=]\s*['\"]?([0-9a-zA-Z\-_]{20,50})['\"]?",
        "severity": "high"
    },
    
    # Add 30+ more patterns as needed
}

# ====================== RECOVERY SCANNER IMPLEMENTATION ======================
class GitRecoveryScanner:
    def __init__(self):
        self.report = {
            "metadata": {
                "tool": "GitGuard Recovery Scanner",
                "version": "2.0",
                "scan_date": datetime.utcnow().isoformat()
            },
            "recovered_items": [],
            "exposed_secrets": []
        }
        self._setup_environment()

    def _setup_environment(self):
        """Prepare scanning environment"""
        Path(CONFIG["TEMP_DIR"]).mkdir(exist_ok=True)
        Path(CONFIG["REPORT_DIR"]).mkdir(exist_ok=True)

    def scan_repository(self, repo_url: str):
        """Full repository scanning and recovery process"""
        repo_name = self._extract_repo_name(repo_url)
        repo_path = Path(CONFIG["TEMP_DIR"]) / repo_name
        
        try:
            print(f"\n\033[1m[+] Beginning forensic scan of {repo_url}\033[0m")
            
            if not self._clone_repository(repo_url, repo_path):
                return False

            # Execute all recovery methods
            recovery_results = self._execute_recovery_methods(repo_path)
            
            if recovery_results["found_items"]:
                self._generate_recovery_report(repo_name)
                print(f"\033[1;32m[+] Found {len(recovery_results['secrets'])} secrets in recovered content\033[0m")
            else:
                print(f"\033[1;33m[-] No recoverable items found in {repo_name}\033[0m")

            return recovery_results["found_items"]
            
        except Exception as e:
            print(f"\033[1;31m[!] Critical error during scan: {e}\033[0m")
            return False
        finally:
            if not recovery_results.get("found_items", False) and CONFIG["CLEANUP_EMPTY_REPOS"]:
                shutil.rmtree(repo_path, ignore_errors=True)

    def _execute_recovery_methods(self, repo_path: Path) -> Dict:
        """Execute all configured recovery methods"""
        results = {
            "found_items": False,
            "secrets": []
        }
        
        # 1. Recover Deleted Files
        if CONFIG["RECOVERY_METHODS"]["DELETED_FILES"]:
            deleted_files = self._recover_deleted_files(repo_path)
            if deleted_files:
                results["found_items"] = True
                results["secrets"].extend(self._scan_recovered_content(deleted_files, "deleted_files"))

        # 2. Find Dangling Objects
        if CONFIG["RECOVERY_METHODS"]["DANGLING_OBJECTS"]:
            dangling_objects = self._find_dangling_objects(repo_path)
            if dangling_objects:
                results["found_items"] = True
                results["secrets"].extend(self._scan_recovered_content(dangling_objects, "dangling_objects"))

        # 3. Check Stashes
        if CONFIG["RECOVERY_METHODS"]["STASHES"]:
            stashes = self._recover_stashes(repo_path)
            if stashes:
                results["found_items"] = True
                results["secrets"].extend(self._scan_recovered_content(stashes, "stashes"))

        # 4. Find Lost Commits
        if CONFIG["RECOVERY_METHODS"]["LOST_COMMITS"]:
            lost_commits = self._find_lost_commits(repo_path)
            if lost_commits:
                results["found_items"] = True
                results["secrets"].extend(self._scan_recovered_content(lost_commits, "lost_commits"))

        # 5. Check Reset Branches
        if CONFIG["RECOVERY_METHODS"]["RESET_BRANCHES"]:
            reset_branches = self._check_reset_branches(repo_path)
            if reset_branches:
                results["found_items"] = True
                results["secrets"].extend(self._scan_recovered_content(reset_branches, "reset_branches"))

        return results

    def _recover_deleted_files(self, repo_path: Path) -> List[Dict]:
        """Recover files deleted in Git history"""
        print("  \033[34m[+] Recovering deleted files from history\033[0m")
        deleted_items = []
        
        try:
            # Get list of all deleted files
            deleted = subprocess.run(
                ["git", "-C", str(repo_path), "log", "--diff-filter=D", "--summary", "--all"],
                capture_output=True, text=True, timeout=CONFIG["GIT_OPERATION_TIMEOUT"]
            ).stdout

            for line in deleted.splitlines():
                if "delete mode" in line:
                    file_path = line.split("delete mode")[1].strip()
                    try:
                        # Find last commit that had this file
                        last_commit = subprocess.run(
                            ["git", "-C", str(repo_path), "rev-list", "-n", "1", "HEAD", "--", file_path],
                            capture_output=True, text=True
                        ).stdout.strip()

                        if last_commit:
                            # Recover file content
                            content = subprocess.run(
                                ["git", "-C", str(repo_path), "show", f"{last_commit}:{file_path}"],
                                capture_output=True, text=True
                            ).stdout

                            deleted_items.append({
                                "type": "deleted_file",
                                "path": file_path,
                                "commit": last_commit[:7],
                                "content": content
                            })
                    except subprocess.SubprocessError:
                        continue

        except subprocess.SubprocessError as e:
            print(f"  \033[31m[!] Error recovering deleted files: {e}\033[0m")
        
        return deleted_items

    def _find_dangling_objects(self, repo_path: Path) -> List[Dict]:
        """Find and recover dangling Git objects"""
        print("  \033[34m[+] Searching for dangling objects\033[0m")
        dangling_items = []
        
        try:
            # Find all unreachable objects
            dangling = subprocess.run(
                ["git", "-C", str(repo_path), "fsck", "--unreachable", "--no-reflogs"],
                capture_output=True, text=True, timeout=CONFIG["GIT_OPERATION_TIMEOUT"]
            ).stdout

            for line in dangling.splitlines():
                if "unreachable blob" in line or "dangling blob" in line:
                    blob_hash = line.split()[-1]
                    try:
                        # Get object content
                        content = subprocess.run(
                            ["git", "-C", str(repo_path), "cat-file", "-p", blob_hash],
                            capture_output=True, text=True
                        ).stdout

                        dangling_items.append({
                            "type": "dangling_object",
                            "hash": blob_hash,
                            "content": content
                        })
                    except subprocess.SubprocessError:
                        continue

        except subprocess.SubprocessError as e:
            print(f"  \033[31m[!] Error finding dangling objects: {e}\033[0m")
        
        return dangling_items

    # [Additional recovery methods (_recover_stashes, _find_lost_commits, etc.)]
    # ... (implementation similar to the methods above)

    def _scan_recovered_content(self, items: List[Dict], source_type: str) -> List[Dict]:
        """Scan recovered content for exposed secrets"""
        found_secrets = []
        
        for item in items:
            content = item.get("content", "")
            for name, config in RECOVERY_PATTERNS.items():
                matches = re.finditer(config["pattern"], content, re.IGNORECASE)
                for match in matches:
                    context = content[max(0, match.start()-200):match.end()+200]
                    found_secrets.append({
                        "type": name,
                        "source": f"{source_type}:{item.get('path', item.get('hash', 'unknown'))}",
                        "value": match.group(0),
                        "context": context,
                        "severity": config["severity"]
                    })

        return found_secrets

    def _generate_recovery_report(self, repo_name: str):
        """Generate comprehensive recovery report"""
        report_path = Path(CONFIG["REPORT_DIR"]) / f"{repo_name}_recovery_report.json"
        with open(report_path, "w") as f:
            json.dump(self.report, f, indent=2)
        print(f"  \033[34m[+] Recovery report saved to {report_path}\033[0m")

    # [Helper methods (_clone_repository, _extract_repo_name, etc.)]
    # ... (implementation similar to previous versions)

# ====================== MAIN EXECUTION ======================
def main():
    parser = argparse.ArgumentParser(description="GitGuard Recovery Scanner - Forensic Git History Analysis")
    parser.add_argument("repository", help="Git repository URL to scan")
    parser.add_argument("--output", default=CONFIG["REPORT_DIR"], help="Output directory for reports")
    parser.add_argument("--keep-empty", action="store_true", help="Keep repos with no findings")
    args = parser.parse_args()

    CONFIG["REPORT_DIR"] = args.output
    CONFIG["CLEANUP_EMPTY_REPOS"] = not args.keep_empty
    
    scanner = GitRecoveryScanner()
    scanner.scan_repository(args.repository)

if __name__ == "__main__":
    main()

