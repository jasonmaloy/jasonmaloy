# monte.py
# Calculates an approximation of pi through a Monte Carlo trial
# Jason Maloy
# February 16 2015
import random
import math

def dartTrial(darts):
    # RETURNS the result of the dart experiment for darts thrown
    hits = 0
    for i in range(1, darts+1):
        x = random.uniform(-1,1)        # Coordinate values for darts thrown
        y = random.uniform(-1,1)
        if x*x + y*y <= 1: hits += 1    # Distance formula. x*x + y*y == sqrt(x*x+y*y) if  
                                        # x*x + y*y <=1
    return ((hits*4)/darts)
    

def main():
    done = False
    while not done:
        try:
            darts = eval(input( "How many darts would you like to throw? "))
            if darts < 0: print( "Must be a positive integer" )
            else:
                print("The result of pi from this experiment is %f" %dartTrial(darts))
                done = True
        except ZeroDivisionError: print( "Must be non-zero" )
        except NameError: print( "Must be a number" )
        except TypeError: print( "Must be a positive integer" )

main()

