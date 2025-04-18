


(define (problem logistics-c2-s2-p15-a1)
(:domain logistics-strips)
(:objects a0 
          c0 c1 
          t0 t1 
          l0-0 l0-1 l1-0 l1-1 
          p0 p1 p2 p3 p4 p5 p6 p7 p8 p9 p10 p11 p12 p13 p14 
)
(:init
    (AIRPLANE a0)
    (CITY c0)
    (CITY c1)
    (TRUCK t0)
    (TRUCK t1)
    (LOCATION l0-0)
    (in-city  l0-0 c0)
    (LOCATION l0-1)
    (in-city  l0-1 c0)
    (LOCATION l1-0)
    (in-city  l1-0 c1)
    (LOCATION l1-1)
    (in-city  l1-1 c1)
    (AIRPORT l0-0)
    (AIRPORT l1-0)
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
    (at t0 l0-1)
    (at t1 l1-0)
    (at p0 l1-1)
    (at p1 l0-0)
    (at p2 l0-1)
    (at p3 l1-1)
    (at p4 l0-1)
    (at p5 l0-0)
    (at p6 l0-0)
    (at p7 l0-0)
    (at p8 l1-0)
    (at p9 l0-1)
    (at p10 l1-0)
    (at p11 l0-0)
    (at p12 l0-0)
    (at p13 l0-0)
    (at p14 l1-1)
    (at a0 l1-0)
)
(:goal
    (and
        (at p0 l1-0)
        (at p1 l0-1)
        (at p2 l1-1)
        (at p3 l1-0)
        (at p4 l0-1)
        (at p5 l0-0)
        (at p6 l0-0)
        (at p7 l0-0)
        (at p8 l0-1)
        (at p9 l0-0)
        (at p10 l0-1)
        (at p11 l0-1)
        (at p12 l1-1)
        (at p13 l1-1)
        (at p14 l1-1)
    )
)
)


