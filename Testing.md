# How we would test!

### **Test case 1: Creating and listing alarms** 
<p>This test case will ensure that alarms can be created and listed correctly:<br></p>

1. Create an alarm, show alarms. Expected result is the alarm just created showing in the terminal with the corerct information. 
2. Create 4 other alarms. Then create one more. Expected result  is a message int hte terminal saying that you may only have 5 alarms at the same time.


### **Test case 2: Validation and cancellation of alarms**
<p>This test will ensure that you cannot create invalid alarms, and that alarms can be properly cancelled</p>

1. Create an alarm with a time in the past. Expected result is a message in the terminal says that this is not allowed.
2. List alarms. Expected result is nothing, to ensure that the invalid alarm was not created.
3. Create an 2 alarms, 1 and 2 minutes from now. Cancel the first alarm. List alarms. Expected result is information about the second alarm printed in the terminal. The test must ensure that this is the correct remaining alarm.

### Test case 3
<p>This test case will ensure that alarms are killed after new input is entered</p>

1. Create an alarm and wait until it rings/cancel it.
2. Type some new input in the terminal (i.e. "l" to display all current alarms)
3. Open another terminal and use top() to verify that no zombie-processes are running
### Test case 4
