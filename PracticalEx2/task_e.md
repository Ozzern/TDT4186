# Task E

## The problem: Path Traversal Attacks
A security issue is that the user can access paths outside the current working directory using the "../" syntax. A naive solution to prevent this problem is to search the path supplied by the user for two dots ("..") in succession and deny the request if an instance is found. However, this is not sufficient as there are several ways to access files that should not be accessible. You can use URL encodings (%2e%2e%2f) or absolute paths (filename=/etc/passwd). This problem therefore requires a more sophisticated response. 

## The solution: Verify the user input
Another fairly simple solution is to have a whitelist of accessible files. In our case, if the server was configured to supply files from the "doc" directory, we would have a list of files in that directory and compare the user input with that. We could also "canonicalize" the path, and verify that the path starts with our base directory.

## Source
(i) https://portswigger.net/web-security/file-path-traversal