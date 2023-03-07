#include <iostream>
#include <iomanip>

using namespace std;

int main() {
    // constant and variable capital
    // surplus divided between profit for capitalist, amount of new constant and variable capital available to add
    // value of yearly product
    double constant, variable, surplus, profit, add_cc, add_vc, val_of_year_prod;
    
    // rate of accumulation of constant capital and variable capital
    // rate of exploitation
    double racc, ravc, roe;

    // percentage of surplus value consumed by capitalist and for accumulation
    // rate of profit
    double por, poa, rop;

    // assume roe is constant at 1:1 (1 V: 1 S)
    roe = 1.0;

    // assume racc is 10% and ravc is 5%
    racc = 1.1;
    ravc = 1.05;

    // initial conditions
    constant = 200000;
    variable = 100000;
    add_cc = 20000;
    add_vc = 5000;

    // some external factors personally added
    double total_profit = 0;
    double current_occ = constant / variable;

    cout << fixed << setprecision(2) << "Year" << "\t|\t"
        << "constant" << "\t|\t" << "variable" << "\t|\t" << "profit" << "\t|\t"
        << "add_cc" << "\t|\t" << "add_vc" << "\t|\t" << "val_of_year_prod" << "\t|\t"
        << "por" << "\t|\t" << "poa" << "\t|\t" << "rop" << "\t|\t" << "occ" << "\t|\t" << "total profit" << "\t|\t" << endl;


    for (int i = 1; i <= 300; i++) {
        // derive surplus
        surplus = roe * variable;

        // derive profit
        // if profit is less than zero, then set profit to zero and restrict the add_cc and add_vc 
        // otherwise deduct the normal accumulation from surplus as usual
        if (surplus < add_cc + add_vc) {
            // when profits are negative, first dip into savings, and then cut future investments
            /* EXPERIMENTAL ---- DEVALUE CONSTANT CAPITAL BY APPROX 5% */
            constant = 0.90 * constant;
            variable = 0.95 * variable;

            // the savings of the capitalists are completely dried
            if (total_profit <= 0) {
                
                /* EXPERIMENTAL ---- SPLIT SURPLUS AS ACCORDING TO LAST POR*/

                // divide the surplus in such a way to match the organic composition of capital and maintain the rate of profit
                // we want the ratio of added capital to value set to maintain the current organic composition of capital
                // that means that two equations must be satisfied:
                // dC/dV = OCC    and   dc + dV = S
                // this gets us -> dV = S/(1+O)
                add_vc = surplus / (1 + current_occ);
                add_cc = surplus - add_vc;

                cout << add_cc - (surplus * current_occ / (1 + current_occ)) << endl;
                if (-0.05 > add_cc - (surplus * current_occ / (1 + current_occ)) || 
                    add_cc - (surplus * current_occ / (1 + current_occ)) > 0.05 ) {
                        cout << "problemo" << endl;
                    }

                profit = 0;
            } else {
                total_profit = total_profit + profit;
                profit = 0;
            }
        } else {
            profit = surplus - (add_cc + add_vc);
            total_profit = total_profit + profit;
        }

        // if the ratio of the accumulated constant capital to the variable capital deviates by more than a factor of two
        // to the ratio of the added constant capital to the added variable capital, then adjust it to match
        if (add_cc / (current_occ * add_vc) > 2) {
            double temp = add_cc + add_vc;

            // assume 90% of current occ and 10% of occ derived from added capital, constant and variable
            double ideal_occ = 0.9 * current_occ + 0.1 * add_cc / add_vc;

            add_vc = temp / (ideal_occ + 1);
            add_vc = temp - add_vc;
        }

        // derive value of yearly product
        val_of_year_prod = constant + variable + surplus;

        // derive percentages of surplus value consumed by capitalist profits and accumulation
        por = profit / surplus;
        poa = (add_cc + add_vc) / surplus;

        // derive rate of profit
        rop = surplus / (constant + variable);

        // now print out the equations a table form
        cout << fixed << setprecision(2) << i << "\t\t|\t"
            << constant << "\t|\t" << variable << "\t|\t" << profit << "\t|\t"
            << add_cc << "\t|\t" << add_vc << "\t|\t" << val_of_year_prod << "\t|\t"
            << por << "\t|\t" << poa << "\t|\t" << rop << "\t|\t" << current_occ << "\t|\t" << total_profit << "\t|\t"<< endl;

        // update for the next cycle
        constant = constant + add_cc;
        variable = variable + add_vc;

        /* EXPERIMENTAL ---- CALCUATE CURRENT VALUE COMPOSITION OF CAPITAL SO THAT NEXT GENERATION IS INFLUENCED BY IT */
        current_occ = constant / variable;


        add_cc = add_cc * racc;
        add_vc = add_vc * ravc;
    }

    return 0;
}