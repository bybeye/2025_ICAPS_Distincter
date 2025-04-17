<div align="center">

# Learning Efficiency Meets Symmetry Breaking

</div>

Official implementation of Learning Efficiency Meets Symmetry Breaking (ICAPS 2025).

Learning-based planners leveraging Graph Neural Networks can learn search guidance applicable to large search spaces, yet their potential to address symmetries remains largely unexplored. In this paper, we introduce a graph representation of planning problems allying learning efficiency with the ability to detect symmetries, along with two pruning methods, action pruning and state pruning, designed to manage symmetries during search. The integration of these techniques into Fast Downward achieves a first-time success over LAMA on the latest IPC learning track dataset.


# Experiments

## Requirements


1. Install singularity

2. Build the containers by executing

```
sudo singularity distincter-container.sif distincter-container.def
```

3. Build Fast Downward 

```
singularity exec --nv --no-home --bind /home/xxx/:/mnt/ /home/xxx/2025_ICAPS_Distincter/distincter-container.sif bash -c "cd /mnt/2025_ICAPS_Distincter/planners/downward_gpu/ && rm -rf builds/release && python build.py"
```


## Data
PDDL and datasets are placed within the "benchmarks" directory.


## Train and Evaluate


1. Blocksworld with action prunning

```
singularity exec --nv --no-home --bind /home/xxx/:/mnt/ /home/xxx/2025_ICAPS_Distincter/distincter-container.sif bash -c "cd /mnt/2025_ICAPS_Distincter/ && python train.py --pruning action --seed 1 --nlayers 3 --domain blocksworld"
```

2. Childsnack with action prunning

```
singularity exec --nv --no-home --bind /home/xxx/:/mnt/ /home/xxx/2025_ICAPS_Distincter/distincter-container.sif bash -c "cd /mnt/2025_ICAPS_Distincter/ && python train.py --pruning action --seed 1 --nlayers 3 --domain childsnack"
```

3. Ferry with action prunning

```
singularity exec --nv --no-home --bind /home/xxx/:/mnt/ /home/xxx/2025_ICAPS_Distincter/distincter-container.sif bash -c "cd /mnt/2025_ICAPS_Distincter/ && python train.py --pruning action --seed 1 --nlayers 3 --domain ferry"
```

4. Floortile without prunning

```
singularity exec --nv --no-home --bind /home/xxx/:/mnt/ /home/xxx/2025_ICAPS_Distincter/distincter-container.sif bash -c "cd /mnt/2025_ICAPS_Distincter/ && python train.py --pruning none --seed 1 --nlayers 3 --domain floortile"
```

5. Miconic without prunning

```
singularity exec --nv --no-home --bind /home/xxx/:/mnt/ /home/xxx/2025_ICAPS_Distincter/distincter-container.sif bash -c "cd /mnt/2025_ICAPS_Distincter/ && python train.py --pruning none --seed 1 --nlayers 3 --domain miconic"
```

6. Rovers with state prunning

```
singularity exec --nv --no-home --bind /home/xxx/:/mnt/ /home/xxx/2025_ICAPS_Distincter/distincter-container.sif bash -c "cd /mnt/2025_ICAPS_Distincter/ && python train.py --pruning state --seed 1 --nlayers 3 --domain rovers"
```

7. Satellite with state prunning

```
singularity exec --nv --no-home --bind /home/xxx/:/mnt/ /home/xxx/2025_ICAPS_Distincter/distincter-container.sif bash -c "cd /mnt/2025_ICAPS_Distincter/ && python train.py --pruning state --seed 1 --nlayers 3 --domain satellite"
```

8. Sokoban with three types of prunning

```
singularity exec --nv --no-home --bind /home/xxx/:/mnt/ /home/xxx/2025_ICAPS_Distincter/distincter-container.sif bash -c "cd /mnt/2025_ICAPS_Distincter/ && python train.py --pruning ablation --seed 1 --nlayers 7 --domain sokoban"
```

9. Spanner with three types of prunning

```
singularity exec --nv --no-home --bind /home/xxx/:/mnt/ /home/xxx/2025_ICAPS_Distincter/distincter-container.sif bash -c "cd /mnt/2025_ICAPS_Distincter/ && python train.py --pruning ablation --seed 1 --nlayers 4 --domain spanner"
```

10. Transport with three types of prunning

```
singularity exec --nv --no-home --bind /home/xxx/:/mnt/ /home/xxx/2025_ICAPS_Distincter/distincter-container.sif bash -c "cd /mnt/2025_ICAPS_Distincter/ && python train.py --pruning ablation --seed 1 --nlayers 3 --domain transport"
```



# License and Contributing
- This README is formatted based on [paperswithcode](https://github.com/paperswithcode/releasing-research-code) 
- Feel free to post issues via Github.

# Reference
If you find the code useful in your research, please consider citing our paper:

<pre>
@inproceedings{
    bai2025distincter,
    title={Learning Efficiency Meets Symmetry Breaking},
    author={Bai, Yingbin and Thiebaux, Sylvie and Trevizan, Felipe},
    booktitle={ICAPS},
    year={2025},
}
</pre>
