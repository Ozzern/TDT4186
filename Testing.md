# How we would test!
(No state is kept between these test cases, they all begin with a clean slate.)

### **Test case 1: Creating and listing alarms** 
<p>This test case will ensure that alarms can be created and listed correctly:<br></p>

1. Create an alarm at a time 5 minutes form now. The program should create an instance of the AlarmStruct with the correct date/time and add a pointer to it in an array. Expected result is the following message in the terminal: "Scheduling alarm in 300 seconds"
2. Type 'l' to list current alarms. When recieving the 'l'-input the program should call the list()-function. Expected result is the alarm just created showing in the terminal with the correct information: "Alarm 1: \<weekday month dayofthemonth hh:mm:ss year\>"
3. Create 4 other alarms. Then create one more. The program should detect that you would have too many alarms and not call the scheduele()-function as usual. Expected result  is a message in the terminal saying that you may only have 5 alarms at the same time: "You cannot schedule more than 5 alarms simultaneously. Please cancel an alarm before setting a new one".


### **Test case 2: Validation and cancellation of alarms**
<p>This test will ensure that you cannot create invalid alarms, and that alarms can be properly cancelled</p>

1. Create an alarm with a time in the past. Expected result is a message in the terminal says that this is not allowed.
2. List alarms. Expected result is nothing, to ensure that the invalid alarm was not created.
3. Create an 2 alarms, 1 and 2 minutes from now. Cancel the first alarm. List alarms. Expected result is information about the second alarm printed in the terminal. The test must ensure that this is the correct remaining alarm.

### **Test case 3: Ringing, killing processes and zombies.**
<p>This test case will ensure that alarms will ring, that their processes are killed and that zombie-processes are killed after new input is entered</p>

1. Create an alarm 10 seconds from the currenct time and wait until it rings. Excpected result is "RING!" being output to the terminal and the sound being played on the computer.
2. Type some new input in the terminal (e.g. "l" to display all current alarms). Expected result is nothing since there should be no active alarms.
3. Open another terminal and use top(). Expected result is that there should be no zombie-processes.

### **Test case 4:  
