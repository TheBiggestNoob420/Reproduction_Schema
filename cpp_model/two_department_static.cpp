#include <iostream>
#include <iomanip>

using namespace std;

int main() {
    // for each department
    // constant and variable capital
    // surplus divided between profit for capitalist, amount of new constant and variable capital available to add
    // value of yearly product
    double constant[2], variable[2], surplus[2], profit[2], add_cc[2], add_vc[2], val_of_year_prod[2];
    
    // for each department
    // rate of accumulation of constant capital and variable capital
    // rate of exploitation, organic 
    double racc[2], ravc[2], roe[2], occ[2];

    // for each department
    // percentage of surplus value consumed by capitalist and for accumulation
    // por: profit / surplus
    // poa: amount for accumulation / surplus
    // rop: surplus / (constant + variable)
    double por[2], poa[2], rop[2];

    // assume roe of department one is constant at 2:1 (2 S: 1 V)
    // assume roe of department two is constant at 1:1 (1 S: 1 V)
    roe[0] = 2.0;
    roe[1] = 1.0;

    // assume racc is 5% and ravc is 3% in department one
    // assume racc is 1% and ravc is 1% in department one
    racc[0] = 1.05;
    ravc[0] = 1.03;
    racc[1] = 1.01;
    ravc[1] = 1.01;
    

    // initial conditions for department one
    constant[0] = 200000;
    variable[0] = 100000;
    add_cc[0] = 20000;
    add_vc[0] = 5000;

    // initial conditions for department two
    constant[1] = 1000;
    variable[1] = 500;
    add_cc[1] = 10;
    add_vc[1] = 5;

    // the general equations for the two department 
    // first calculate surplus value
    // S1{t} = roe1*V1{t}
    // S2{t} = roe2*V2{t}
    //
    // second calculate value of yearly product
    // C1{t} + V1{t} + S1{t} = K1{t}
    // C2{t} + V2{t} + S2{t} = K2{t}
    //
    // then calculate the profits
    // P1{t} = S1{t} - (dC1{t} + dV1{t})
    // P2{t} = S2{t} - (dC2{t} + dV2{t})
    // 
    // third calculate the amount of variable and constant capital accumulated
    // dC1{t} = racc1 * C1{t} , dV1 = ravc1 * V1{t}
    // dC2 = racc2 * C2{t} , dV2 = ravc2 * V2{t}
    //

    for (int i = 1; i <= 100; i++) {
        // derive surplus
        surplus[0] = roe[0] * variable[0];
        surplus[1] = roe[1] * variable[1];



        // if profit is less than zero, then set profit to zero and restrict the add_cc and add_vc 
        // otherwise deduct the normal accumulation from surplus as usual
        if (surplus[0] < add_cc[0] + add_vc[0]) {
            // profits are negative
            profit[0] = surplus[0] - (add_cc[0] + add_vc[0]);
            
            // since profits are negative, if amount of added variable capital is greater than negative profits,
            // then take the extra from them add_vc and subtract it from add_vc
            // if this exceeds add_vc, take it from add_cc as well
            if (add_vc[0] + profit[0] > 0) {
                add_vc[0] = add_vc[0] + profit[0];
            } else {
                add_vc[0] = add_vc[0] + profit[0];
                add_cc[0] = add_cc[0] + add_vc[0];
                add_vc[0] = 0;
            }
            profit[0] = 0;
        } else {
            profit[0] = surplus[0] - (add_cc[0] + add_vc[0]);
        }

        // derive value of yearly product
        val_of_year_prod[0] = constant[0] + variable[0] + surplus[0];

        // derive percentages of surplus value consumed by capitalist profits and accumulation
        por[0] = profit[0] / surplus[0];
        poa[0] = (add_cc[0] + add_vc[0]) / surplus[0];

        // derive rate of profit
        rop[0] = surplus[0] / (constant[0] + variable[0]);

        // now print out the equations a table form
        cout << fixed << setprecision(2)
            << constant[0] << "\t|\t" << variable[0] << "\t|\t" << profit[0] << "\t|\t"
            << add_cc[0] << "\t|\t" << add_vc[0] << "\t|\t" << val_of_year_prod[0] << "\t|\t"
            << por[0] << "\t|\t" << poa[0] << "\t|\t" << rop[0] << "\t|\t" << endl;


        // update for the next cycle
        constant[0] = constant[0] + add_cc[0];
        variable[0] = variable[0] + add_vc[0];

        add_cc[0] = add_cc[0] * racc[0];
        add_vc[0] = add_vc[0] * ravc[0];

    }

    return 0;
}