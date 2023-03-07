import math

"""
Plotting the capital accumulation rates (both variable and constant) for each capitalist produces 4 independent variables
Even worse is the fact that accumulation rates don't change and aren't influenced.
Simply put, there is too much data to find a pattern. Instead, use AI to produce optimal accumulation rates.

To prevent the AI from simply choosing accumulation rates that end up becoming stable, the AI will be blind to other
AI and their information. Transparancy should be implemented in other models (at a given cost). The influence of finance
and the government is ignored in this model.
"""

# this function takes in the model as well as the given conditions for the sector it represents
# and it outputs the rate of accumulation as well as how much should be produced
def compute_rates(model, constant, variable, ):

# this function tests the AI for some given every time you want to recompute the data
def compute(percentage_racc=10, percentage_ravc=5):
    # assumption
    roe = 1.0

    # independent variables
    racc = 1 + percentage_racc / 100
    ravc = 1 + percentage_ravc / 100

    # initial conditions
    constant = 200000
    variable = 100000
    add_cc = 20000
    add_vc = 5000

    total_profit = 0
    current_occ = constant / variable


    year_list = []
    rop_list = []
    occ_list = []
    profit_list = []
    is_cyclical = False


    for i in range(1, 300):
        year = i
        year_list.append(year)


        surplus = roe * variable
        if surplus < add_cc + add_vc:
            constant = 0.90 * constant
            variable = 0.95 * variable

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
        profit_list.append(math.log10(profit + 1))

        # i added this to model the limitations of technology, and it turns out this is necessary for cyclical crisis
        # essentially, this model 
        if add_cc / (current_occ * add_vc) > 2:
            # print(i, "\t", current_occ, "\t", add_cc / add_vc)
            is_cyclical = True
            temp = add_cc + add_vc

            ideal_occ = 0.9 * current_occ + 0.1 * add_cc / add_vc

            add_vc = temp / (ideal_occ + 1)
            add_vc = temp - add_vc
        
        val_of_year_prod = constant + variable + surplus


        por = profit / surplus
        poa = (add_cc + add_vc) / surplus

        rop = surplus / (constant + variable)
        rop_list.append(rop)


        # update for next cycle
        constant = constant + add_cc
        variable = variable + add_vc

        # calculate current occ so that the next generation can utilize the data
        occ_list.append(current_occ)
        current_occ = constant / variable

        # increase the amount accumulated for the next cycle
        add_cc = add_cc * racc
        add_vc = add_vc * ravc
    
    # print("Current Model is ", is_cyclical)

    diff_profit = [0]
    for i in range(len(profit_list) - 1):
        diff_profit.append(profit_list[i + 1] - profit_list[i])


    # find abs max and divide whole thing by it
    max = 0
    for e in diff_profit:
        if abs(e) > max:
            max = abs(e)

    for i in range(len(diff_profit)):
        diff_profit[i] = diff_profit[i] / max


    # temp do it for profits as well
    max = 0
    for e in profit_list:
        if abs(e) > max:
            max = abs(e)

    for i in range(len(profit_list)):
        profit_list[i] = profit_list[i] / max


    # return the lists
    return (year_list, rop_list, occ_list, profit_list, diff_profit, is_cyclical)
