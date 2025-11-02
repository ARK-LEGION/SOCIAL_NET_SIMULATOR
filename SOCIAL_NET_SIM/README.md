# &nbsp;			  **SocialNet Simulator**			

# 			       **(README)** 







## INPUT FORMAT:



The program reads input from the standard input stream (stdin).

A shell script "run.sh" is provided to compile the code on various systems.



On Linux / macOS / Git-Bash / WSL:

(first change the directory to where the code and the shell script are stored using the cd command)

make script executable:



Type the following sequence of commands:



&nbsp;chmod +x "run.sh"

&nbsp;./run.sh

&nbsp;./socialnet



The user can now give input according to the supported commands (case-sensitive)

\- ADD\_USER <username>

\- ADD\_FRIEND <username1> <username2>

\- LIST\_FRIENDS <username>

\- SUGGEST\_FRIENDS <username> <n>

\- DEGREES\_OF\_SEPARATION <username1> <username2>

\- ADD\_POST <username> <post content>  (content is the rest of the line after username)

\- OUTPUT\_POSTS <username> <n>  (n = -1 to output all posts)



Note: Usernames and post contents are not case-sensitive. For example, the usernames ARJUN and arjun are identical.



## COMMAND REFERENCES:



1. &nbsp;ADD\_USER <username> Adds a new user to the network, initially with no friends and no posts.
2. &nbsp;ADD\_FRIEND <username1> <username2> Establishes a bidirectional friendship between two existing
    usernames.
3. &nbsp;LIST\_FRIENDS <username> Prints an alphabetically sorted list of the specified username’s friends.
4. &nbsp;SUGGEST\_FRIENDS <username> <N> Recommends up to N new friends who are ”friends of a friend”
    but not already friends.
    	• Ranking: Recommendations are ranked by the number of mutual friends (descending). Ties are
   	  broken by alphabetical order of usernames.
    	• Edge Cases: If N is 0, output nothing. If fewer than N candidates exist, list all available.
5. DEGREES OF SEPARATION <username1> <username2> Calculates the length of the shortest path of

&nbsp;  friendships between two usernames. If no path exists, reports -1.

6\. ADD\_POST <username> "<post content>" Add a post whose content is the post to the posts created by the specified user.

&nbsp;  content string,

7\. OUTPUT\_POSTS <username> <N> Output the most recent N posts of the user, in reverse chronological

&nbsp;  order. If N is-1,  outputs all the posts by the user. If there are fewer than N posts by the user,

&nbsp;  then list all available posts.



## Input / Output notes:



\- Usernames and post content are normalized to lowercase internally.

\- ADD\_POST stores posts with a timestamp (time(0)); posts are returned in reverse-chronological order using an inorder traversal of the AVL tree and reversing the output.

\- DEGREES\_OF\_SEPARATION returns the shortest path length (BFS), or -1 if either user does not exist or they are not connected.

\- SUGGEST\_FRIENDS computes friends-of-friends, prioritizing by mutual friend count, then alphabetically.



## Data structures:



* **post:** stores message text and a creation timestamp.
* **AVL\_TREE:** stores a user's posts keyed by timestamp to keep posts balanced (log n inserts). Inorder traversal yields oldest→newest.
* **user:** owns an AVL\_TREE of posts and provides ADD\_POST (stores content lowercased) and OUTPUT\_POSTS (prints newest first by reversing inorder).
* **SocialNetwork:** maintains users in a map keyed by normalized (lowercase, trimmed) usernames and friendships as an adjacency‑list graph (vector<vector<int>>). Also keeps an index->user map for lookups.





## Command I/O:



Reads commands from stdin, one command per line (ADD\_USER, ADD\_FRIEND, ADD\_POST, LIST\_FRIENDS, SUGGEST\_FRIENDS, DEGREES\_OF\_SEPARATION, OUTPUT\_POSTS).

Username lookup is case‑insensitive; post content is converted to lowercase before storage.

ADD\_POST consumes the remainder of the input line as the post content.

OUTPUT\_POSTS <user> <n> prints up to n most recent posts (n = -1 => all).





## Assumptions \& limitations:



\- Usernames are unique; ADD\_USER prints "User already exists" on duplicates.

\- No persistent storage — all state is in-memory for the program run.

\- Timestamps use time(0), so posts created in the same second may be ordered arbitrarily.

\- No deletion operations present.





## Complexity (high level):



* ADD\_USER: O(1) amortized (map insert O(log U) where U = users).
* ADD\_POST: O(log P) per user.
* LIST\_FRIENDS: O(F log F) to sort F friends for output.
* DEGREES\_OF\_SEPARATION / SUGGEST\_FRIENDS: O(U + F\_total) BFS-style.
