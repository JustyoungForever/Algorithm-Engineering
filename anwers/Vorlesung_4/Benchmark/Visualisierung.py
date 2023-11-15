# -*- coding: utf-8 -*-
"""
Created on Thu Nov 16 06:19:13 2023

@author: Pan
"""

import matplotlib.pyplot as plt
import pandas as pd


df_threads = pd.read_csv('threads_speedup.csv')


plt.figure(figsize=(10, 6))
plt.plot(df_threads['Threads'], df_threads['QS_Speedup'], label='QS Speedup', color='blue')
plt.plot(df_threads['Threads'], df_threads['GNU_Speedup'], label='GNU Speedup', color='red')
plt.xlabel('Threads (Number of Threads)')
plt.ylabel('Speedup')
plt.title('Threads vs Speedup')
plt.legend(loc='upper right')
plt.grid(True)
plt.show()


df_array = pd.read_csv('array_speedup.csv')


plt.figure(figsize=(10, 6))
plt.plot(df_array['Size'], df_array['QS_Speedup'], label='QS Speedup', color='blue')
plt.plot(df_array['Size'], df_array['GNU_Speedup'], label='GNU Speedup', color='red')
plt.xlabel('Array Size')
plt.ylabel('Speedup')
plt.title('Array Size vs Speedup')
plt.xscale('log')  
plt.legend(loc='upper right')
plt.grid(True)
plt.show()
