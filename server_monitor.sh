#!/bin/bash

########################################################
# Log activities and alerts to a log file
########################################################
log_func(){
    echo "$(date +"%Y-%m-%d %H:%M:%S") - $1" >> ./server_monitor_logs/server_monitor.log
}

########################################################
# Checking disk usage percentage
########################################################
disk_usage_func(){

    disk_usage=$(df -h / | awk 'NR==2 {print $5}' | tr -d '%')

    # checking disk threshold
    if [ $disk_usage -gt 30 ]; then
        log_func "Disk usage is above 30%. Sending alert..."
    fi
}

########################################################
# Checking CPU usage percentage
########################################################
cpu_usage_func(){
    
    cpu_usage=$(top -bn1 | grep "Cpu(s)" | awk '{print $2 + $4}' | awk -F. '{print $1}')

    # checking cpu threshold
    if [ $cpu_usage -gt 50 ]; then
        log_func "CPU usage is above 50%. Sending alert..."
    fi
}

########################################################
# Checking available memory percentage
########################################################
memory_usage_func(){

    mem_usage=$(free | awk 'NR==2 {printf "%.0f", $4/$2 * 100}')

    # checking remaining memory
    if [ $mem_usage -lt 10 ]; then
        log_func "Available memory is below 10%. Sending alert..."
    fi
}

########################################################
# Log rotation
########################################################
log_file_func(){
    
    log_file="./server_monitor_logs/server_monitor.log"
    log_size=$(du -m "$log_file" | awk '{print $1}')

    # checking log file size
    if [ $log_size -gt 5 ]; then
        mv ./server_monitor_logs/server_monitor.log ./server_monitor_logs/server_monitor.log.old
        touch ./server_monitor_logs/server_monitor.log
        log_func "Log file size exceeds 5MB. Rotating log..."
    fi
}

########################################################
mkdir -p ./server_monitor_logs
touch ./server_monitor_logs/server_monitor.log
while [ true ]; do
    disk_usage_func
    cpu_usage_func
    memory_usage_func
    log_file_func
    sleep 20
    # echo "here"
done