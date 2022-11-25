# primes.py
# Lists the first n primes, then counts how many twin primes are among them
# Jason Maloy
# February 21 2015

def isPrime(n):
    # RETURNS True if n is a prime
    isPrime = True
    while isPrime:
        for i in range(2, n):
            if n % i == 0: isPrime = False
        break
    return isPrime

def listNthPrimes(n):
    # RETURNS a list containing the first Nth primes
    primes = list()
    number = 2
    while len(primes) < n:
        if isPrime(number):
            primes.append(number)
            number += 1
        else: number += 1
    return primes

def checkTwinPrimes(x, y):
    # RETURNS True if x and y are twin primes
    if y - x == 2: return True
    else: return False
        

def main():
    n = eval(input( "Enter a positive integer: "))
    primes = listNthPrimes(n)
    numTwinPrimes = 0
    for i in range(0, len(primes)-1):
        if checkTwinPrimes(primes[i], primes[i+1]): numTwinPrimes += 1
    print( "The first %d primes are: " %n)
    print(primes)
    print( "Amongst these are %d twin primes." %numTwinPrimes)
    
    

main()
