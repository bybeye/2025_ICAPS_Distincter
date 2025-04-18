


(define (problem logistics-c3-s2-p25-a2)
(:domain logistics-strips)
(:objects a0 a1 
          c0 c1 c2 
          t0 t1 t2 
          l0-0 l0-1 l1-0 l1-1 l2-0 l2-1 
          p0 p1 p2 p3 p4 p5 p6 p7 p8 p9 p10 p11 p12 p13 p14 p15 p16 p17 p18 p19 p20 p21 p22 p23 p24 
)
(:init
    (AIRPLANE a0)
    (AIRPLANE a1)
    (CITY c0)
    (CITY c1)
    (CITY c2)
    (TRUCK t0)
    (TRUCK t1)
    (TRUCK t2)
    (LOCATION l0-0)
    (in-city  l0-0 c0)
    (LOCATION l0-1)
    (in-city  l0-1 c0)
    (LOCATION l1-0)
    (in-city  l1-0 c1)
    (LOCATION l1-1)
    (in-city  l1-1 c1)
    (LOCATION l2-0)
    (in-city  l2-0 c2)
    (LOCATION l2-1)
    (in-city  l2-1 c2)
    (AIRPORT l0-0)
    (AIRPORT l1-0)
    (AIRPORT l2-0)
    (OBJ p0)
    (OBJ p1)
    (OBJ p2)
    (OBJ p3)
    (OBJ p4)
    (OBJ p5)
    (OBJ p6)
    (OBJ p7)
    (OBJ p8)
    (OBJ p9)
    (OBJ p10)
    (OBJ p11)
    (OBJ p12)
    (OBJ p13)
    (OBJ p14)
    (OBJ p15)
    (OBJ p16)
    (OBJ p17)
    (OBJ p18)
    (OBJ p19)
    (OBJ p20)
    (OBJ p21)
    (OBJ p22)
    (OBJ p23)
    (OBJ p24)
    (at t0 l0-0)
    (at t1 l1-0)
    (at t2 l2-1)
    (at p0 l2-0)
    (at p1 l2-1)
    (at p2 l1-0)
    (at p3 l0-1)
    (at p4 l0-1)
    (at p5 l1-0)
    (at p6 l1-0)
    (at p7 l1-0)
    (at p8 l0-1)
    (at p9 l1-0)
    (at p10 l1-1)
    (at p11 l1-0)
    (at p12 l0-1)
    (at p13 l1-1)
    (at p14 l2-1)
    (at p15 l0-1)
    (at p16 l0-1)
    (at p17 l1-0)
    (at p18 l0-0)
    (at p19 l1-0)
    (at p20 l0-0)
    (at p21 l2-0)
    (at p22 l2-1)
    (at p23 l0-1)
    (at p24 l0-0)
    (at a0 l2-0)
    (at a1 l1-0)
)
(:goal
    (and
        (at p0 l2-1)
        (at p1 l0-1)
        (at p2 l0-0)
        (at p3 l2-1)
        (at p4 l0-1)
        (at p5 l0-1)
        (at p6 l2-1)
        (at p7 l1-1)
        (at p8 l0-0)
        (at p9 l0-1)
        (at p10 l1-0)
        (at p11 l2-1)
        (at p12 l2-0)
        (at p13 l2-1)
        (at p14 l1-1)
        (at p15 l0-0)
        (at p16 l1-1)
        (at p17 l1-1)
        (at p18 l0-0)
        (at p19 l2-0)
        (at p20 l1-1)
        (at p21 l1-0)
        (at p22 l1-1)
        (at p23 l1-0)
        (at p24 l2-1)
    )
)
)


