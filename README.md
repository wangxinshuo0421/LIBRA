# LIBRA simulation
This is an NS-3 simulator for LIBRA: Multi-Path Transport in Distributed Training Data Center Network (ICNP' 2025). It also includes the implementation of HPCC, DCQCN, TIMELY, DCTCP, PFC, ECN and Broadcom shared buffer switch.

This project is based on the [HPCC NS3](https://github.com/alibaba-edu/High-Precision-Congestion-Control) code framework and supports additional features such as multi-path spray, out-of-order rdma packet direct write, sender bitmap, selective repeat, and switch congestion notification.

## NS-3 simulation
The ns-3 simulation is under `simulation/`. Refer to the README.md under it for more details.

## Traffic generator
The traffic generator is under `traffic_gen/`. Refer to the README.md under it for more details.

## Analysis
We provide a few analysis scripts under `analysis/` to view the packet-level events, and analyzing the fct in the same way as [HPCC](https://liyuliang001.github.io/publications/hpcc.pdf) Figure 11.
Refer to the README.md under it for more details.

## Questions
For technical questions, please create an issue in this repo, so other people can benefit from your questions. 
You may also check the issue list first to see if people have already asked the questions you have :)

For other questions, please contact Xinshuo Wang (wangxs@dsp.ac.com).
