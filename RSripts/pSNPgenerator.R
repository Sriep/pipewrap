library("seqinr")  
fastaInFilename <-"Sc8915bp_cs.fasta"
fastaOutFileName <- "pSNPgen.csv"
changesOutFileName <- "changes.csv"
numTR <- 200
pSNPpct <- c(100,10,6,4,3,2,1,0.8,0.77,0.6,0.5,0)
genPSNPperPct <- 10
cmdArgs = commandArgs()
numArgs = length(cmdArgs)
if (is.element('-i', cmdArgs))
{
    if (match('-i', cmdArgs)<numArgs)
            fastaInFilename = cmdArgs[match('-i', cmdArgs) +1]   
}
if (is.element('-o', cmdArgs))
{
    if (match('-o', cmdArgs)<numArgs)
            fastaOutFileName = cmdArgs[match('-o', cmdArgs) +1]   
}
if (is.element('-c', cmdArgs))
{
    if (match('-c', cmdArgs)<numArgs)
            changesOutFileName = cmdArgs[match('-c', cmdArgs) +1]   
}
if (is.element('-t', cmdArgs))
{
    if (match('-t', cmdArgs)<numArgs)
            numTR = strtoi(cmdArgs[match('-t', cmdArgs) +1])   
}
if (is.element('-p', cmdArgs))
{
    if (match('-p', cmdArgs)<numArgs)
            genPSNPperPct = strtoi(cmdArgs[match('-p', cmdArgs) +1])
}
if (is.element('-b', cmdArgs))
{
    if (match('-b', cmdArgs)<numArgs)
#            pSNPpct = strtoi(cmdArgs[(match('-b', cmdArgs) +1):numArgs])
            pSNPpct = as.double(cmdArgs[(match('-b', cmdArgs) +1):numArgs]) 
}
cs <- read.fasta(file = fastaInFilename)
sizeTR <- length(cs[[1]])
csTR = c(cs[[1]])
for (i in 2:numTR)
{
        csTR = c(csTR, cs[[1]])
}
numPct <- length(pSNPpct)
numPsnpToGenerate = numPct*genPSNPperPct
rndPositons <- sample.int(sizeTR, numPsnpToGenerate, replace = FALSE)
position = 1:numPsnpToGenerate
csBase = 1:numPsnpToGenerate
pSNPBase = 1:numPsnpToGenerate
frequency = 1:numPsnpToGenerate 
StartTR = 1:numPsnpToGenerate 
currentTR <- 0
for (i in 1:numPct)
{
        for (j in 1:genPSNPperPct)
        {
                pSNPindex <- (i-1)*genPSNPperPct + j
                 pSNPpos <- rndPositons[pSNPindex]
                oldBase <- csTR[pSNPpos]
                if (('A' == oldBase) || ('a' == oldBase))
                {
                        newBase <- 'G'
                }else
                {
                        newBase <- 'A'
                }
                repeatsPSNP <- round(pSNPpct[i]*numTR/100)
                position[pSNPindex] <- pSNPpos
                csBase[pSNPindex] <- oldBase
                pSNPBase[pSNPindex] <- newBase
                frequency[pSNPindex] <- pSNPpct[i]
                StartTR[pSNPindex] <- currentTR%%numTR               
                for (k in 1:repeatsPSNP)
                {
                        nextPos <- (currentTR %% numTR)*sizeTR + pSNPpos
                        currentTR <- currentTR + 1
                        csTR[nextPos] <- newBase
                }
        }
}
write.fasta(csTR, "pSNPadded", nbchar = 80, fastaOutFileName)
changes = data.frame(position,csBase,pSNPBase,frequency,StartTR)
write.table(changes, changesOutFileName, sep = "\t")

