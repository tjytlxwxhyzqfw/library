import logging
import subprocess
import os
import sys


def errquit(msg, logger=None):
	if logger is None:
		print msg
	else:
		logger.error(msg)
	sys.exit(1)

def cutoff(msg):
	print "--- %s --->"%msg

def output(cmd):
	p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT);
	return p.communicate()[0].strip()

def fwrite(string, path):
	with open(path, "w") as fil:
		fil.write(string)

def fread(path, raw=False):
	with open(path, "r") as inf:
		lns = [x for x in inf]
		if not raw:
			lns = [x.strip() for x in lns]
			lns = filter(lambda x: x, lns)
		return lns

def fcont(path):
	return "".join(fread(path, raw=True))

def getlogger(name):
	logger = logging.getLogger(name)

	loghandler = logging.StreamHandler()
	logformatter = logging.Formatter('%(asctime)s %(filename)s: %(funcName)s: %(lineno)d %(levelname)s %(message)s')
	loghandler.setFormatter(logformatter)

	logger.addHandler(loghandler)
	logger.setLevel(logging.INFO)

	return logger

