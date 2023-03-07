import matplotlib.pyplot as plt
import numpy as np


"""
This program runs models of accumulation from 1 to 50 percent for both accumulation types. 
For each model (suppose 10% constant and 5% variable) it determines the nature of this model
which is put into 4 categories
"""

# call this function every time you want to recompute the data
def compute(percentage_racc=10, percentage_ravc=5):
    # assumption
    roe = 1.0

    # independent variables
    racc = 1 + percentage_racc / 100
    ravc = 1 + percentage_ravc / 100

    # initial conditions
    constant = 200000 - 1
    variable = 100000
    add_cc = 20000
    add_vc = 5000

    total_profit = 0
    current_occ = constant / variable


    year_list = []
    profit_list = []
    occ_adjusted = False

    for i in range(1, 300):
        year = i
        year_list.append(year)


        surplus = roe * variable
        if surplus < add_cc + add_vc:
            # devaluation of constant capital at 10% and variable capital at 5%
            rdev_cc = 0.1
            rdev_vc = 0.05
            constant = (1.00 - rdev_cc) * constant
            variable = (1.00 - rdev_vc) * variable

            if total_profit <= 0:
                add_vc = surplus / (1 + current_occ)
                add_cc = surplus - add_vc

                profit = 0
            else:
                total_profit = total_profit + profit
                profit = 0
        else:
            profit = surplus - (add_cc + add_vc)
            total_profit = total_profit + profit

        # after calculating profit and adjusting the variables given as necessary
        # add profit onto the list, AFTER taking the log of it
        profit_list.append(np.log10(profit + 1))


        if add_cc / (current_occ * add_vc) > 2:
            occ_adjusted = True
            temp = add_cc + add_vc

            ratio = 0.9
            ideal_occ = (ratio) * current_occ + (1 - ratio) * add_cc / add_vc

            add_vc = temp / (ideal_occ + 1)
            add_vc = temp - add_vc


        # update for next cycle
        constant = constant + add_cc
        variable = variable + add_vc

        # calculate current occ so that the next generation can utilize the data
        current_occ = constant / variable

        # increase the amount accumulated for the next cycle
        add_cc = add_cc * racc
        add_vc = add_vc * ravc

    # return the lists
    return (year_list, profit_list, occ_adjusted)


def classify(prof_list, occ_adjusted):
    '''
    We want to classify the different simulations:

    (1) Models with no crisis and no downturns
    (2) Models with breakdown then becomes (1)
    (3) Models with cyclical non-crisis
    (4) Models with cylical crisis
    (5) Models that breakdown completely
    '''

    # if the model has never had to adjust its real_occ to be between the current_occ and occ of the accumulated capital
    # then it has no crisis nor downturns
    # with (1) and (2), there is no adjustment of occ necessary
    if not occ_adjusted:
        # it is either (1) or (2)
        count = 0
        for profit in prof_list:
            if profit == 0:
                count = count + 1
            else:
                if count > 0:
                    count = count - 1
            if count >= 5:
                return 2
        return 1
    else:
        # the occ has been adjusted at least once, meaning that it must be determined if it is cyclical, or if it breaks down
        # the way this is done is to determine the profit cycles

        # first determine if profit ever reaches 0
        zeroprofit = False
        for profit in prof_list:
            if profit == 0:
                zeroprofit = True
         
        # if profit has never been zero, then it is cyclical non-crisis (3)
        if not zeroprofit:
            return 3

        # if profit hits zero at a certain point and remains so for the rest of years the model runs, then
        # the model has hit its breakdown. otherwise, it is cyclical crisis

        length_of_crisis = 0
        longest_crisis = length_of_crisis
        for i in range(len(prof_list)):
            if prof_list[i] == 0:
                length_of_crisis = length_of_crisis + 1
            else:
                if length_of_crisis > longest_crisis:
                    longest_crisis = length_of_crisis
                length_of_crisis = 0


        if length_of_crisis > longest_crisis:
            longest_crisis = length_of_crisis

        # if the length of the longest crisis is 10 or less, it for sure hasn't broken down
        if longest_crisis <= 10:
            return 4
        
        # otherwise, substract 10 from longest_crisis to get the amount of years to check
        # at the end of the list, to confirm that these years were all zero
        # since if they were, then it means that it experienced breakdown

        zeroprofit = True
        longest_crisis = longest_crisis - 10
        for i in range(len(prof_list) - longest_crisis, len(prof_list)):
            if prof_list[i] != 0:
                zeroprofit = False
        
        if zeroprofit == True:
            return 5
        else:
            return 4

    '''
    total = 0
    for profit in prof_list:
        total = total + profit
    
    return total/len(prof_list)
    '''



# main program


fig, ax = plt.subplots()

ax.tick_params(top=True, bottom=False, labeltop=True, labelbottom=False)
ax.set_xlabel('percentage constant capital accumulation')
ax.set_ylabel('percentage variable capital accumulation')

classes = []

for y in range(0, 501, 10):
    line = []
    for x in range(0, 501, 10):
        y_list, p_list, occ_adjusted = compute(x/10, y/10) 

        line.append(classify(p_list, occ_adjusted))
        
    classes.append(line)

ax.imshow(classes, cmap='viridis')

plt.show()
