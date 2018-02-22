
# Part b of HW3 Biostat 615 / Georgios Spyrou

# Money change problem with dynamic programming
# we will use an extra dictionary called stored that will hold
# the values that we dont want to recalculate and so avoid redundant actions

# at first we import the proper libraries like sys and copy
# copy will be usefull in order to use the deepcopy function
import sys
import copy
# the function is already pre-defined at the assignment
def dpChangeMoney(total, units, stored):
    # we pretty much follow the logic as in the first assignment
    # for the money change problem
    # so we just need to modify the code from the solution from hw1b
    # and make it run with dynamic programming
    if (total == 0): # terminating conditions
        return [{}]
    elif (total < 0 ) : # same logic as in the first assignment
        return []
    else:
        length_n = len(units) # length of the money the user enters (the denominations)
        
        if total in stored.keys() : # if the amount(total) in keys
            if length_n in stored[total] :
                return stored[total][length_n] 
            else :
                stored[total][length_n] = []
                for i in range(0,length_n):
                    sols = copy.deepcopy(dpChangeMoney(total-units[i],  # we are using copy.deepcopy
                                                       units[i:length_n], stored)) # per suggestion of the professor
                    for sol in sols: # updating if units in sol
                        if (units[i] in sol):
                            sol[units[i]] = sol[units[i]] + 1 # increment by 1 if in sol
                        else:
                            sol[units[i]] = 1 # assign a new one / it wasnt there before
                        stored[total][length_n].append(sol) # save the solutions at stored!
                return stored[total][length_n] # return our primary dictionary
        else: # if total not in stored.keys()
            stored[total],stored[total][length_n] = {} , []
            for i in range(0,length_n):
                sols = copy.deepcopy(dpChangeMoney(total-units[i], 
                                                   units[i:length_n], stored)) # again we are using copy.deepcopy
                for sol in sols: # updating-same logic as before
                    if (units[i] in sol):
                        sol[units[i]] = sol[units[i]] + 1 # same logic as before
                    else:
                        sol[units[i]] = 1
                    stored[total][length_n].append(sol) # save the solutions at stored!
            return stored[total][length_n] # return prim. dictionary

# this indeed run faster than the one in hw1
# 23 seconds the dynamic programming one
# 49 seconds the one from hw1 
# thus we see that the DP one is really faster(more than half faster)

# finally these are also predefined at the assignment
# so we just implement them , something that will make available to run the 
# program through the scs server
dollars = int(sys.argv[1])
bills = [int(x) for x in sys.argv[2:len(sys.argv)]]
ways = dpChangeMoney(dollars, bills, {})
print("There are " + str(len(ways)) + " possible ways to make "+str(dollars))
for way in ways:
    print(sorted(way.items()))










