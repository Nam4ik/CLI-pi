<h1 align = center> CLI-π </h1> 
<div align = center>
    
![CMake Build Status](https://img.shields.io/github/actions/workflow/status/Nam4ik/CLI-pi/cmake-single-platform.yml?logo=cmake&label=build&style=flat-square)
[![License](https://img.shields.io/badge/License-BSD_2--Clause-blue.svg)](https://opensource.org/licenses/BSD-2-Clause)
[![C](https://img.shields.io/badge/C-99-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![MPFR](https://img.shields.io/badge/MPFR-4.2.0-blue.svg)](https://www.mpfr.org/)

</div>
<br>

**tpi - C-written, MPFR-based CLI program for calculating pi, licensed by BSD2-clause**

> [!WARNING]
> For the accuracy of calculations, a large MPFR library is used, there may be delays and high processor loads on weak systems.

<br>
<br>
<div align = center> 
    <img src = "./README/sample.gif"> 
</div>
<br>
<br>


The program calculates the pi number with high accuracy. 

In infinite mode, something like AOT compilation is used, first ~10 million decimal places are calculated, and then they are converted to `char[]` through the same MPFR.

$$100,000,000 \text{ ms} = 100,000 \text{ s} = \frac{100,000}{3600} \text{ hours} \approx 27.78 \text{ h}$$

But if you need more, you can change the len variable and recompile the project with `build.sh` for yourself by setting a different limit. Be careful, the CPU load will be higher.



# How does the calculation work?
Calculations are performed using the Gauss method, which is based on the use of integrals and properties of elliptic integrals. This method, also known as the Gauss-Legandre method, makes it possible to calculate pi with high accuracy using an iterative process.

$$\pi \approx \frac{(a_n + b_n)^2}{4 t_n}$$

In python it looks like: 
```python
def gauss_pi(iterations):
    a = 1.0
    b = 1.0 / math.sqrt(2)
    t = 1.0 / 4.0
    p = 1.0

    for _ in range(iterations):
        a_next = (a + b) / 2
        b = math.sqrt(a * b)
        t -= p * (a - a_next) ** 2
        a = a_next
        p *= 2

    return (a + b) ** 2 / (4 * t)
```

I'm not giving an example from my code because the gnu MPFR syntax is unreadable without an armed eye.
