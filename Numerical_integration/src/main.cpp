#include "../include/midpoint_approximator.hpp"
#include "../include/trapezoidal_approximator.hpp"
#include "../include/simpson_approximator.hpp"

#include <iostream>

using namespace sctoolbox;
int main(int argc, char *argv[])
{
    std::string mode = "";
    if (argc >= 2)
    {
        mode = argv[1];
    }
    if (mode == "demo")
    {
        // First example: function of degree 1
        unsigned int n = 10;
        std::cout << "For all the examples, we will use n = " << n << " number of subintervals." << std::endl;
        double a = 2.0;
        double b = 5.0;
        std::string f = "3*x+2";
        std::cout << "The first integral we will study is integral of " << f << " from " << a << " to " << b << "." << std::endl;

        MidpointApproximator mid1(a, b, f, n);
        TrapezoidalApproximator tra1(a, b, f, n);
        SimpsonApproximator sim1(a, b, f, n);

        std::cout << "According to WolframAlpha, the integral of " << f << " from " << a << " to " << b << " is 37.5." << std::endl
                  << "Let's see the result using the three approximation rules (Midpoint, Trapezoidal, Simpson):" << std::endl;
        try
        {
            std::cout << "Midpoint: " << mid1.approximate() << std::endl;
            std::cout << "Trapezoidal: " << tra1.approximate() << std::endl;
            std::cout << "Simpson: " << sim1.approximate() << std::endl;
        }
        catch (std::invalid_argument &e)
        {
            std::cerr << "Parser Error: " << e.what() << std::endl;
        }
        std::cout << "As you can see, functions of degree 1 are perfectly handled by all three rules." << std::endl;

        // Second example: sin(x)
        a = 4.0;
        b = 6.0;
        f = "sin(x)";
        std::cout << "The second integral we will study is integral of " << f << " from " << a << " to " << b << "." << std::endl;

        MidpointApproximator mid2(a, b, f, n);
        TrapezoidalApproximator tra2(a, b, f, n);
        SimpsonApproximator sim2(a, b, f, n);

        std::cout << "According to WolframAlpha, the integral of " << f << " from " << a << " to " << b << " is approximately -1.6138." << std::endl
                  << "Let's see the result using the three approximation rules:" << std::endl;
        try
        {
            std::cout << "Midpoint: " << mid2.approximate() << std::endl;
            std::cout << "Trapezoidal: " << tra2.approximate() << std::endl;
            std::cout << "Simpson: " << sim2.approximate() << std::endl;
        }
        catch (std::invalid_argument &e)
        {
            std::cerr << "Parser Error: " << e.what() << std::endl;
        }
        std::cout << "The Simpson's rule is better at approximating this result than the other two; this is predictable given how it's the most complex of the three rules." << std::endl;

        // Third example: x^4+3*x^2-cos(x)
        a = 3.2;
        b = 8.5;
        f = "x^4+3*x^2-cos(x)";
        std::cout << "The third integral we will study is integral of " << f << " from " << a << " to " << b << "." << std::endl;

        MidpointApproximator mid3(a, b, f, n);
        TrapezoidalApproximator tra3(a, b, f, n);
        SimpsonApproximator sim3(a, b, f, n);

        std::cout << "According to WolframAlpha, the integral of " << f << " from " << a << " to " << b << " is 9387.5." << std::endl
                  << "Let's see the result using the three approximation rules:" << std::endl;
        try
        {
            std::cout << "Midpoint: " << mid3.approximate() << std::endl;
            std::cout << "Trapezoidal: " << tra3.approximate() << std::endl;
            std::cout << "Simpson: " << sim3.approximate() << std::endl;
        }
        catch (std::invalid_argument &e)
        {
            std::cerr << "Parser Error: " << e.what() << std::endl;
        }
        std::cout << "Once again, Simpson's rule is the best; the Midpoint rule is the second-best, always under-evaluating the result."
                  << " The Trapezoidal rule always over-evaluates the result instead, and it's the least accurate of the three rules." << std::endl;

        // Third example: x^4+3*x^2-cos(x) with n = 1000
        n = 1000;
        std::cout << "Let's now try to approximate again the last integral with a much higher n = " << n << "." << std::endl;

        MidpointApproximator mid4(a, b, f, n);
        TrapezoidalApproximator tra4(a, b, f, n);
        SimpsonApproximator sim4(a, b, f, n);

        try
        {
            std::cout << "Midpoint: " << mid4.approximate() << std::endl;
            std::cout << "Trapezoidal: " << tra4.approximate() << std::endl;
            std::cout << "Simpson: " << sim4.approximate() << std::endl;
        }
        catch (std::invalid_argument &e)
        {
            std::cerr << "Parser Error: " << e.what() << std::endl;
        }
        std::cout << "With a much higher n, we can see that the three rules converge to the correct result, as expected." << std::endl;
    }
    else
    {
        while (true)
        {
            unsigned int n;
            std::string f;
            double a, b;

            std::cout << "Enter your desired function (or q to leave): ";
            std::getline(std::cin, f);
            if (f == "q")
            {
                return 0;
            }
            std::cout << "Enter the lower limit of intergration: ";
            while (true)
            {
                if (std::cin >> a)
                {
                    break;
                }
                else
                {
                    std::cout << "Invalid input! Please insert a number: ";
                    std::cin.clear(); // Reset errors
                    while (std::cin.get() != '\n')
                        ; // Clean previous input
                }
            }
            std::cout << "Enter the upper limit of intergration: ";
            while (true)
            {
                if (std::cin >> b)
                {
                    break;
                }
                else
                {
                    std::cout << "Invalid input! Please insert a number: ";
                    std::cin.clear(); // Reset errors
                    while (std::cin.get() != '\n')
                        ; // Clean previous input
                }
            }
            std::cout << "Enter the number of subintervals for the approximations: ";
            while (true)
            {
                if (std::cin >> n)
                {
                    break;
                }
                else
                {
                    std::cout << "Invalid input! Please insert an integer number: ";
                    std::cin.clear(); // Reset errors
                    while (std::cin.get() != '\n')
                        ; // Clean previous input
                }
            }

            std::cout << "Here are your inputs: a = " << a << ", b = " << b << ", f = " << f << ", n = " << n << std::endl;

            MidpointApproximator mid(a, b, f, n);
            TrapezoidalApproximator tra(a, b, f, n);
            SimpsonApproximator sim(a, b, f, n);

            try
            {
                std::cout << "Midpoint: " << mid.approximate() << std::endl;
                std::cout << "Trapezoidal: " << tra.approximate() << std::endl;
                std::cout << "Simpson: " << sim.approximate() << std::endl;
            }
            catch (std::invalid_argument &e)
            {
                std::cerr << "Parser Error: " << e.what() << std::endl;
            }
            std::cin.ignore(256, '\n'); // Clean previous input
        }
    }
}