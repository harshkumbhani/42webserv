#!/usr/bin/python

import os

query_string = os.environ.get("QUERY_STRING", "")

params = {}
if query_string:
	pairs = query_string.split("&")
	for pair in pairs:
		if '=' in pair:
			parts = pair.split("=", 1)
			if len(parts) == 2:
				key, value = parts
				params[key] = value
			elif len(parts) == 1:
				params[part[0]] = ""

param_value = params.get("param", "Hello World!")
num_value = params.get("num", "1")

try:
	num_value = int(num_value)
except ValueError:
	num_value = 1

for i in range(num_value):
	print(f"<p>{param_value}</p>")