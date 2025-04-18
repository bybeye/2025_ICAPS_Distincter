


(define (problem logistics-c4-s3-p60-a4)
(:domain logistics-strips)
(:objects a0 a1 a2 a3 
          c0 c1 c2 c3 
          t0 t1 t2 t3 
          l0-0 l0-1 l0-2 l1-0 l1-1 l1-2 l2-0 l2-1 l2-2 l3-0 l3-1 l3-2 
          p0 p1 p2 p3 p4 p5 p6 p7 p8 p9 p10 p11 p12 p13 p14 p15 p16 p17 p18 p19 p20 p21 p22 p23 p24 p25 p26 p27 p28 p29 p30 p31 p32 p33 p34 p35 p36 p37 p38 p39 p40 p41 p42 p43 p44 p45 p46 p47 p48 p49 p50 p51 p52 p53 p54 p55 p56 p57 p58 p59 
)
(:init
    (AIRPLANE a0)
    (AIRPLANE a1)
    (AIRPLANE a2)
    (AIRPLANE a3)
    (CITY c0)
    (CITY c1)
    (CITY c2)
    (CITY c3)
    (TRUCK t0)
    (TRUCK t1)
    (TRUCK t2)
    (TRUCK t3)
    (LOCATION l0-0)
    (in-city  l0-0 c0)
    (LOCATION l0-1)
    (in-city  l0-1 c0)
    (LOCATION l0-2)
    (in-city  l0-2 c0)
    (LOCATION l1-0)
    (in-city  l1-0 c1)
    (LOCATION l1-1)
    (in-city  l1-1 c1)
    (LOCATION l1-2)
    (in-city  l1-2 c1)
    (LOCATION l2-0)
    (in-city  l2-0 c2)
    (LOCATION l2-1)
    (in-city  l2-1 c2)
    (LOCATION l2-2)
    (in-city  l2-2 c2)
    (LOCATION l3-0)
    (in-city  l3-0 c3)
    (LOCATION l3-1)
    (in-city  l3-1 c3)
    (LOCATION l3-2)
    (in-city  l3-2 c3)
    (AIRPORT l0-0)
    (AIRPORT l1-0)
    (AIRPORT l2-0)
    (AIRPORT l3-0)
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
    (OBJ p25)
    (OBJ p26)
    (OBJ p27)
    (OBJ p28)
    (OBJ p29)
    (OBJ p30)
    (OBJ p31)
    (OBJ p32)
    (OBJ p33)
    (OBJ p34)
    (OBJ p35)
    (OBJ p36)
    (OBJ p37)
    (OBJ p38)
    (OBJ p39)
    (OBJ p40)
    (OBJ p41)
    (OBJ p42)
    (OBJ p43)
    (OBJ p44)
    (OBJ p45)
    (OBJ p46)
    (OBJ p47)
    (OBJ p48)
    (OBJ p49)
    (OBJ p50)
    (OBJ p51)
    (OBJ p52)
    (OBJ p53)
    (OBJ p54)
    (OBJ p55)
    (OBJ p56)
    (OBJ p57)
    (OBJ p58)
    (OBJ p59)
    (at t0 l0-2)
    (at t1 l1-1)
    (at t2 l2-0)
    (at t3 l3-1)
    (at p0 l3-2)
    (at p1 l3-0)
    (at p2 l2-2)
    (at p3 l2-1)
    (at p4 l0-0)
    (at p5 l1-0)
    (at p6 l0-0)
    (at p7 l1-2)
    (at p8 l2-1)
    (at p9 l3-2)
    (at p10 l2-0)
    (at p11 l2-0)
    (at p12 l3-1)
    (at p13 l0-2)
    (at p14 l2-0)
    (at p15 l3-2)
    (at p16 l3-2)
    (at p17 l0-0)
    (at p18 l2-2)
    (at p19 l2-2)
    (at p20 l2-0)
    (at p21 l0-1)
    (at p22 l1-1)
    (at p23 l0-2)
    (at p24 l2-2)
    (at p25 l3-2)
    (at p26 l0-0)
    (at p27 l2-2)
    (at p28 l1-2)
    (at p29 l0-0)
    (at p30 l2-2)
    (at p31 l2-0)
    (at p32 l1-1)
    (at p33 l2-2)
    (at p34 l1-0)
    (at p35 l2-1)
    (at p36 l1-0)
    (at p37 l2-1)
    (at p38 l3-2)
    (at p39 l2-1)
    (at p40 l3-1)
    (at p41 l1-1)
    (at p42 l0-2)
    (at p43 l2-0)
    (at p44 l3-2)
    (at p45 l1-1)
    (at p46 l2-2)
    (at p47 l2-2)
    (at p48 l2-2)
    (at p49 l0-2)
    (at p50 l1-1)
    (at p51 l3-1)
    (at p52 l1-2)
    (at p53 l1-2)
    (at p54 l1-2)
    (at p55 l2-0)
    (at p56 l1-2)
    (at p57 l3-2)
    (at p58 l3-1)
    (at p59 l3-1)
    (at a0 l3-0)
    (at a1 l1-0)
    (at a2 l3-0)
    (at a3 l3-0)
)
(:goal
    (and
        (at p0 l0-0)
        (at p1 l3-1)
        (at p2 l0-2)
        (at p3 l3-2)
        (at p4 l2-1)
        (at p5 l1-0)
        (at p6 l1-1)
        (at p7 l2-0)
        (at p8 l3-2)
        (at p9 l3-0)
        (at p10 l2-2)
        (at p11 l0-0)
        (at p12 l0-1)
        (at p13 l1-2)
        (at p14 l0-2)
        (at p15 l2-2)
        (at p16 l1-1)
        (at p17 l0-1)
        (at p18 l3-2)
        (at p19 l3-0)
        (at p20 l2-1)
        (at p21 l1-0)
        (at p22 l1-0)
        (at p23 l2-0)
        (at p24 l2-2)
        (at p25 l0-1)
        (at p26 l2-1)
        (at p27 l0-0)
        (at p28 l3-0)
        (at p29 l2-0)
        (at p30 l2-1)
        (at p31 l1-0)
        (at p32 l3-1)
        (at p33 l0-0)
        (at p34 l0-0)
        (at p35 l0-0)
        (at p36 l0-1)
        (at p37 l1-2)
        (at p38 l0-2)
        (at p39 l2-0)
        (at p40 l0-2)
        (at p41 l2-0)
        (at p42 l3-1)
        (at p43 l1-1)
        (at p44 l1-2)
        (at p45 l2-1)
        (at p46 l0-2)
        (at p47 l3-0)
        (at p48 l0-1)
        (at p49 l2-0)
        (at p50 l3-0)
        (at p51 l2-2)
        (at p52 l3-0)
        (at p53 l0-0)
        (at p54 l3-0)
        (at p55 l1-0)
        (at p56 l1-2)
        (at p57 l2-2)
        (at p58 l2-1)
        (at p59 l2-0)
    )
)
)


