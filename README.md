# Mean, Variance, Standard deviation

- Write a parallel program to calculate the above mean, variance, and standard
  deviation using the below algorithm:

1. Each process calculates its local sum on its portion of data. If the division
   result has a remainder, then send this remainder to the last process (the
   process that has the rank "n-1" or let the master process work on it).
2. The master process calculates the mean (dividing the total sum by the size
   of the elements) and sends it to all processes.
3. Each process calculates the squared difference on its portion of data.
4. The master process then calculates the variance (dividing the total
   squared difference by the size of the elements).
5. The master process calculates the standard deviation by getting the
   square root of the variance and prints the results..
