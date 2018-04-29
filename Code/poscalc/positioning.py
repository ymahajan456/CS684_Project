from math import exp, log, sqrt

dist_params = [
    (0.16636, 23.4867),
    (0.14263, 28.3397),
    (0.18668, 24.1223),
    (0.17244, 22.5671)]

def rssi2dist_exp(rssi):
    """
    Get distance value from RSSI value

    curve approximated as exponential: r = C-Ae^(Bx)
    A = 42.7799
    B = -0.018033
    C = 46.8431
    """
    lA, iB, C = 3.7561, -55.4539, 46.8431

    d = (log(C - rssi) - lA)*iB
    return d

# Linear approx
def rssi2dist(rssi, anchor=2):
    m, c = dist_params[anchor]
    return (rssi - c)/m

def circle_intersect(o1, r1, o2, r2):
    """
    (x-x1)^2 + (y-y1)^2 = r1^2
    (x-x2)^2 + (y-y2)^2 = r2^2

    y = mx + c
    x^2 - 2 x x1 + x1^2 + m^2x^2 + 2 mx (c-y1) + (c-y1)^2 = r1^2
    """
    x1, y1 = o1
    x2, y2 = o2

    # line co-effs
    m = (x2 - x1)/(y1 - y2)
    c = (x1**2-x2**2 + y1**2-y2**2 - r1**2+r2**2)/2/(y1-y2)
    # print(m, c)

    # quad eq co-effs
    a1, a2, a3 = m**2 + 1, 2*m*(c-y1) - 2*x1, (c-y1)**2 + x1**2 - r1**2
    # print(a1, a2, a3)
    D = sqrt(a2**2 - 4*a1*a3)
    s1 = (-a2 + D)/a1/2
    s2 = (-a2 - D)/a1/2
    # print(s1, s2)

    sy1 = m*s1 + c
    sy2 = m*s2 + c

    return [(s1,sy1), (s2,sy2)]

def location(L=150, A1_rssi=30, A2_rssi=40, A3_rssi=42, A4_rssi=40):
    """
    A2--A3
    |   |
    A1--A4
    """
    
    r1 = rssi2dist(A1_rssi, 0)
    r2 = rssi2dist(A2_rssi, 1)
    r3 = rssi2dist(A3_rssi, 2)
    r4 = rssi2dist(A4_rssi, 3)

    # print(r1, r2, r3, r4)

    I12 = circle_intersect((0, 0), r1, (0, L + 0.001), r2)
    I23 = circle_intersect((0, L + 0.001), r2, (L, L), r3)
    I34 = circle_intersect((L, L), r3, (L, 0.001), r4)
    I41 = circle_intersect((L, 0.001), r4, (0, 0), r1)

    # print(I12, I23, I34, I41)

    p12 = I12[0] if I12[0][0] >= 0 else I12[1]
    p23 = I23[0] if I23[0][1] <= 150 else I23[1]
    p34 = I34[0] if I34[0][0] <= 150 else I34[1]
    p41 = I41[0] if I41[0][1] >= 0 else I41[1] 

    x = (p12[0] + p23[0] + p34[0] + p41[0])/4
    y = (p12[1] + p23[1] + p34[1] + p41[0])/4

    return (x,y)
