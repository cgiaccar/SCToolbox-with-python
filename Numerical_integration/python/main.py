import build.approximator as ap
import sys

mode = ""
if (len(sys.argv) >= 2):
    mode = sys.argv[1]
if (mode == "demo"):

    # First example: function of degree 1
    n = 10
    print(f"For all the examples, we will use {n = } number of subintervals.")
    a = 2.0
    b = 5.0
    f = "3*x+2"
    print(f"The first integral we will study is integral of {f} from {a} to {b}.")

    mid1 = ap.MidpointApproximator(a, b, f, n)
    tra1 = ap.TrapezoidalApproximator(a, b, f, n)
    sim1 = ap.SimpsonApproximator(a, b, f, n)

    print(f"According to WolframAlpha, the integral of {f} from {a} to {b} is 37.5. Let's see the result using the three approximation rules (Midpoint, Trapezoidal, Simpson):")
    
    try: 
        print(f"Midpoint: {mid1.approximate()}")
        print(f"Trapezoidal: {tra1.approximate()}")
        print(f"Simpson: {sim1.approximate()}")
    except Exception as e: print(f"{type(e).__name__}: {e}")

    print("As you can see, functions of degree 1 are perfectly handled by all three rules.")

    # Second example: sin(x)
    a = 4.0
    b = 6.0
    f = "sin(x)"
    print(f"The second integral we will study is integral of {f} from {a} to {b}.")

    mid2 = ap.MidpointApproximator(a, b, f, n)
    tra2 = ap.TrapezoidalApproximator(a, b, f, n)
    sim2 = ap.SimpsonApproximator(a, b, f, n)

    print(f"According to WolframAlpha, the integral of {f} from {a} to {b} is approximately -1.6138. Let's see the result using the three approximation rules:")
    
    try: 
        print(f"Midpoint: {mid2.approximate()}")
        print(f"Trapezoidal: {tra2.approximate()}")
        print(f"Simpson: {sim2.approximate()}")
    except Exception as e: print(f"{type(e).__name__}: {e}")

    print("The Simpson's rule is better at approximating this result than the other two; this is predictable given how it's the most complex of the three rules.")

    # Third example: x^4+3*x^2-cos(x)
    a = 3.2
    b = 8.5
    f = "x^4+3*x^2-cos(x)"
    print(f"The third integral we will study is integral of {f} from {a} to {b}.")

    mid3 = ap.MidpointApproximator(a, b, f, n)
    tra3 = ap.TrapezoidalApproximator(a, b, f, n)
    sim3 = ap.SimpsonApproximator(a, b, f, n)

    print(f"According to WolframAlpha, the integral of {f} from {a} to {b} is 9387.5. Let's see the result using the three approximation rules:")
    
    try: 
        print(f"Midpoint: {mid3.approximate()}")
        print(f"Trapezoidal: {tra3.approximate()}")
        print(f"Simpson: {sim3.approximate()}")
    except Exception as e: print(f"{type(e).__name__}: {e}")

    print("Once again, Simpson's rule is the best; the Midpoint rule is the second-best, always under-evaluating the result. The Trapezoidal rule always over-evaluates the result instead, and it's the least accurate of the three rules.")

    # Third example: x^4+3*x^2-cos(x) with n = 1000
    n = 1000
    print(f"Let's now try to approximate again the last integral with a much higher {n = }.")

    mid4 = ap.MidpointApproximator(a, b, f, n)
    tra4 = ap.TrapezoidalApproximator(a, b, f, n)
    sim4 = ap.SimpsonApproximator(a, b, f, n)

    try: 
        print(f"Midpoint: {mid4.approximate()}")
        print(f"Trapezoidal: {tra4.approximate()}")
        print(f"Simpson: {sim4.approximate()}")
    except Exception as e: print(f"{type(e).__name__}: {e}")

    print("With a much higher n, we can see that the three rules converge to the correct result, as expected.")

# if "demo" argument is not provided:
else:
    while True:

        f = input("Enter your desired function (or q to leave): ")
        if (f == "q"):
            quit()

        a = input("Enter the lower limit of intergration: ")
        while True:
            if a.isnumeric():
                a = float(a)
                break
            else:
                a = input("Invalid input! Please insert a number: ")
        
        b = input("Enter the upper limit of intergration: ")
        while True:
            if b.isnumeric():
                b = float(b)
                break
            else:
                b = input("Invalid input! Please insert a number: ")
        
        n = input("Enter the number of subintervals for the approximations: ")
        while True:
            if n.isnumeric():
                n = int(n)
                break
            else:
                n = input("Invalid input! Please insert a number: ")

        print(f"Here are your inputs: {a = }, {b = }, {f = }, {n = }")

        mid = ap.MidpointApproximator(a, b, f, n)
        tra = ap.TrapezoidalApproximator(a, b, f, n)
        sim = ap.SimpsonApproximator(a, b, f, n)

        try: 
            print(f"Midpoint: {mid.approximate()}")
            print(f"Trapezoidal: {tra.approximate()}")
            print(f"Simpson: {sim.approximate()}")
        except Exception as e: print(f"{type(e).__name__}: {e}")