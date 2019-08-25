#!/usr/bin/env Rscript

cat("\n")

invisible(x <- read.table("output.txt", header = FALSE))
invisible(unlist(x))

# Average
result.mean <- mean(x$V1)
cat(result.mean)
cat("\n")

# Standard Dev
s<- sd(x$V1)
cat(s)
cat("\n")

# Normal
error <- qnorm(0.975)*s/sqrt(100)
cat(error)
cat("\n")

# T-Student
#error2 <- qt(0.975,df=100-1)*s/sqrt(100)
#cat(error2)
#cat("\n")
