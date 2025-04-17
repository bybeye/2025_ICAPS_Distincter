import os
import time
import logging
import itertools
import subprocess
from datetime import datetime
from multiprocessing import Pool, Value

logger = logging.getLogger('Logger')


class result_statistics:
    def __init__(self):
        self.success = Value('i', 0)
        self.failed = Value('i', 0)
        self.total_failed = Value('i', 0)


def search_finished_correctly(log_file):
    with open(log_file, 'r', encoding='utf-8') as f:
        log = f.read()
        finished_correctly = (
            'timed out after' in log
            or 'Solution found.' in log
            or 'Time limit has been reached.' in log
        )

        successful = 'Solution found.' in log
        return finished_correctly, successful


def run_eval(df, pf, domain, difficulty, problem_name, model_file, test_log_file, rep, pruning, timeout):
    start = datetime.now()
    task_name = domain + '_' + difficulty + '_' + problem_name
    logger.info(f"Running test {task_name}")

    with open(test_log_file, 'w', encoding='utf-8') as log_file:
        try:
            cmd = fd_cmd(df=df, pf=pf, model_path=model_file, task_name=task_name, rep=rep, pruning=pruning, timeout=timeout)
            logger.info(cmd)
            subprocess.run(cmd, check=True, stdout=log_file, stderr=subprocess.STDOUT)
        except subprocess.CalledProcessError as e:
            if int(e.returncode) != 23:
                logger.exception(f"Child was terminated by signal - {e.returncode}")
        except OSError:
            logger.exception(f'OSError: {cmd}')

    with open(test_log_file, 'r', encoding='utf-8') as f:
        read_log = f.read()
        solved = 'Solution found.' in read_log
        if solved:
            logger.info(f'Cost time: {(datetime.now() - start).total_seconds()}, {task_name} Solved')
            with stat.failed.get_lock() and stat.success.get_lock():
                # failed.value = 0
                stat.success.value += 1
        else:
            logger.info(f'Cost time: {(datetime.now() - start).total_seconds()}, {task_name} Failed')
            with stat.failed.get_lock() and stat.total_failed.get_lock():
                stat.failed.value += 1
                stat.total_failed.value += 1


def fd_cmd(df, pf, model_path, task_name, search='gbbfs', rep='ilg', pruning='', timeout=1800):
    description = f"{task_name}_{os.path.basename(model_path).replace('.dt', '')}"
    sas_file = f'./logs/aux/{task_name}.sas'
    plan_file = f'./logs/plans/{description}.plan'

    if search == 'gbbfs':
        search = 'batch_eager_greedy'
    elif search == 'gbfs':
        search = 'eager_greedy'
    else:
        raise NotImplementedError

    return ['python',
            './planners/downward_gpu/fast-downward.py',
            '--translate-time-limit', '600',
            '--search-time-limit', str(timeout),
            '--sas-file', sas_file,
            '--plan-file', plan_file,
            df, pf,
            '--search', f'{search}([ml2(rep_type="{rep}", pruning="{pruning}", model_path="{model_path}", domain_file="{df}", instance_file="{pf}")])']


def error_callback(error):
    logger.exception(f"Evaluator Poorl Error: {error}")


def testing_evaluate(args):
    global stat

    stat = result_statistics()
    desc = args.model_file.replace('jit_', '').replace('.dt', '')
    model_file = args.model_dir + args.model_file

    df = f'./benchmarks/{args.domain}/domain.pddl'  # domain file
    test_dir = f'./benchmarks/{args.domain}/testing'
    test_configs = list(itertools.product(['easy', 'medium', 'hard'], [f'p0{i}' for i in range(1, 10)] + [f'p{i}' for i in range(10, 31)],))
    multi_re = []
    with Pool(processes=args.p_num) as pool:
        for difficulty, problem_name in test_configs:
            pf = f'{test_dir}/{difficulty}/{problem_name}.pddl'
            if not os.path.exists(pf):
                logger.error(f'{pf} can not be found! Pass!')
                continue

            test_log_file = f'./logs/{desc}_{difficulty}_{problem_name}.log'
            if os.path.exists(test_log_file):
                os.remove(test_log_file)

            re_async = pool.apply_async(run_eval, args=(df, pf, args.domain, difficulty, problem_name, model_file, test_log_file, args.rep, args.pruning, args.timeout), error_callback=error_callback)
            multi_re.append(re_async)

        while True:
            num_not_finish = len([re for re in multi_re if not re.ready()])
            if stat.failed.value >= args.fail_limit:
                logger.info('Too many failed, stop evaluating more!!!')
                pool.terminate()
                pool.join()
                break

            if num_not_finish == 0:
                logger.info('Finish all tasks.')
                break

            time.sleep(1)

    logger.info(f'{args.domain} coverage is {stat.success.value}, failed: {stat.total_failed.value}, not execute: {num_not_finish}')
    time.sleep(5)

    try:
        pids = subprocess.check_output(['pgrep', 'downward']).decode().strip().split('\n')
        for pid in pids:
            os.system(f'kill -9 {pid}')
    except subprocess.CalledProcessError:
        logger.info('No downward processes are currently running.')

    time.sleep(10)
    return int(stat.success.value)
