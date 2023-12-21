#!/bin/bash
#SBATCH --partition=EPYC
#SBATCH --job-name=my_super_job
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=24
#SBATCH --mem=200gb 
#SBATCH --time=00:02:00
#SBATCH --output=my_job_%j.out 
pwd; hostname; date 
echo "Hello, world !
