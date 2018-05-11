#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


int main(int argc, char const *argv[])
{
	
	//A piece of code that shows how to create 3 children from a single process
	//Note: It reaaaally works!
	pid_t child_1, child_2, child_3;

	child_1 = fork();

	if (child_1 == 0) {
	    /* Child 1 code */
	    printf("Child_1 here! %d\n", (int) getpid());
	} else {
	    child_2 = fork();

	    if (child_2 == 0)
	    {
	    	/* Child 2 Code */
	    	printf("Child_2 here! %d\n", (int) getpid());
	    } else {
	    	child_3 = fork();

	    	if (child_3 == 0)
	    	{
	    		/* Child 3 Code */
	    		printf("Child_3 here! %d\n", (int) getpid());
	    	} else {
	    		/*Parent Code*/
	    		printf("Parent here! %d\n", (int) getpid());
	    		wait(NULL);
	    	}
	    }

	}

	return 0;
}