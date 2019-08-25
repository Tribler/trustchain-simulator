#!/usr/bin/env Rscript

invisible(x <- read.table("result/output.txt", header = FALSE))
invisible(unlist(x))

result.mean <- mean(x$V1)
print(result.mean)

s<- sd(x$V1)
print(s)

error <- qnorm(0.975)*s/sqrt(100)
error
