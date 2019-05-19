I have changed the prompt of my shell in first 2 commits using getcwd command.
tried to implement signal handler but there are still some issues. Handled the signal using signal().
Now i am trying to implement I/O redirection and wrote 2 functions of them. Took help from dreamincode.net in case of I/o redirection. lets hope that it works!!!!!!!!!!
I/o redirection. used freopen() call to redirect input and output between files.
Tried to implement process backgrounding functionality by writing background_process function but there are some errors. 
removed some errors but it's still not working. Now i am trying pipe functionality and some internal commands
Implemented internal commands of exit and cd by writing a internal_commands function
where i implemented these commands using their system calls of exit() and chdir().
Implemented help command
Tried to implement pipe handling but there are some errors i'm trying to
figure out.
Removed some errors ad now going to write pipe handling function
chnaged the check_pipe function completely. NOW using strsep to seperate the
arguments.
Whenever i try to store the success scenerio of check_pipe function, there comes
segmentation fault. 
