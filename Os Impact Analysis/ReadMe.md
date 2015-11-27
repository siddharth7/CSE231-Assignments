The aim of this project is to analyze the impact of OS concepts  and fine-tune the program to run efficiently. Towards this, we will analyze role of concurrency and synchronization on program performance. All the sub-tasks are mandatory and need to be submitted for getting your assignment evaluated.

1. Write a simple counter -non-thread safe. Name the file as Rollno_counter1_nonsafe.c
2. Write a simple thread-safe counter. Name the file as Rollno_counter2_safe.c
3. Write a single thread program that uses the counter1 and counts till one billion and measure the time taken. Name the file as Rollno_stp.c
Do this 50 times (write a program to do it) and compute the average time taken to count one billion with a single thread program.
4. Write a multi-thread program that uses  first the counter1 and then counter2 and counts till one billion. Name the files as Rollno_mtp_c1.c for counter1 and Rollno_mtp_c2.c for counter2.
Increase the number of threads starting from 2 to 10 counting 50 times each (write a program to do it) and compute the average time taken. 
Measure the average time. 
Plot all the averages that you obtained including the one which you computed earlier and explain it in terms of performance and correctness specifically: 1) do you see any relation between number of threads and performance, what is it 2) do you see any relation between number of threads and correctness, what is it - submit the plot and your explanation as a pdf file and name it as Rollno_plot1.pdf.
5. Write a multi-threaded program that uses counter 1, with semaphores to ensure correctness,  to count till billions.  Increase the number of threads starting from 2 to 10 counting 50 times each (write a program to do it) and compute the average time taken. Name the files as Rollno_mtp_c1_sem.c. Plot this with the previous plot and comment on performance and correctness as above - name the file Rollno_plot2.pdf.
