#!/bin/bash

valgrind \
	--leak-check=full \
	--show-leak-kinds=all \
	--verbose \
	--track-origins=yes \
	./$1
