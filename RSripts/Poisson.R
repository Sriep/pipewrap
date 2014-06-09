K <- 1421
N <- 147
Q <- 65
cmdArgs <- commandArgs()
numArgs = length(cmdArgs)
if (is.element('-N', cmdArgs))
{
    if (match('-N', cmdArgs)<numArgs)
            N <- cmdArgs[match('-N', cmdArgs) +1]   
}
if (is.element('-K', cmdArgs))
{
    if (match('-K', cmdArgs)<numArgs)
            K <- cmdArgs[match('-K', cmdArgs) +1]   
}
if (is.element('-Q', cmdArgs))
{
    if (match('-Q', cmdArgs)<numArgs)
            Q <- cmdArgs[match('-Q', cmdArgs) +1] 
}
N# = type.convert(N)
K# = type.convert(K)
Q# = type.convert(Q)
p <- 10^(-1*Q/10)
p
o <- trunc((1/p)-1)
#dpois(0:K-1, lambda = p, log = TRUE)
dpois(0:K-1, lambda = p)
ppois(K-1,lambda= p, lower = TRUE)
#ppois(K,lambda= p, lower = FALSE)


