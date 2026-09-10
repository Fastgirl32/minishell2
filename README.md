# SABRINA HANDLN
- ```>new``` als ```echo -n > new``` werten.  
Auch in Pipes wie ```>new | >new2 | echo hihi``` -> ```echo -n > new | echo -n new2 | echo hihi```

_This project has been created as part of the 42 curriculum by lstarek, saecker._

saecker was in charge for the "input" part, (parsing input into a usable struct and handling heredocs, prompts, history etc structured in the "input" subfolder)  
lstarek was in charge for the "execute" part, (using parsed input to perform logic, structured in the "builtins" and "execute" subfolder).

# DESCRIPTION
Minishell - As beautiful as a shell  
The purpose of minishell is to implement a simple shell / commandline tool.  
The main features include a prompt with history, searching executables and launching them, builtins (echo, ls, pwd...), and handling interrupts.

# INSTRUCTIONS
### Installation: 
``` bash
    git clone <project_url> <folder_name>
```
### Compilation:
Makefile is present. Run in the root directory:
``` bash
    make
```
Additional rules exist on top of those required by the subject.  
``` bash
    make funny      # Compiles, executable displays the custom banner
    make verbose    # Compiles, executable displays the command struct
    make vg         # Runs the program with valgrind and suppression for readline leaks
```
### Execution:
Launch minishell and play around.
``` bash
    ./minishell
    # or if you want valgrind to show relevant leaks:
    make vg
```

# RESOURCES
Listed below are resources used to create the project.  
  
- YOUTUBE Vids:  
[Monitor Child Process State || Waitpid() || Flags = WEXITSTATUS, WCONTINUED, WNOHANG](https://www.youtube.com/watch?v=sPplMz89Ibc)  

- People on campus that helped:  
@kmautner, @flaltens, @fscharf.  

- Readline suppression (make vg) by @kmautner

- Link that leads nowhere that EVERY project needs to have:  
[Very important resource](https://www.shfdjhsfjhsjdfh.sjdfkjskfdj)  