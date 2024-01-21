
def parseTracerouteFile(tracerouteImport, tracerouteExport):
    with open(tracerouteImport, 'r') as file:
        # file.read() reads the whole thing and puts into string, read lines puts it into lines
        lines = file.readlines()
    currentIp = None
    delays = []
# Write a program to parse each line of this file
# and compute the average delay (averaged over the three delay values) for each hop.
# Your program should write a new file that has the IP address of each hop
# (the address within parentheses in each line of traceroute data) and the corresponding average delay.
# Each hop's information should be on a separate line in the new file.

    with open(tracerouteExport, 'w') as file:
    # for each line in the imported file, split it into pieces
        for line in lines:
        # strips the string from having whitespaces and splits them sowe can use it in an array
            parts = line.strip().split()

            #used for if we have empty lines so that we dont have to continue going through the rest of code
            # if not parts or len(parts) < 2:
            if not parts:
                 continue
            # print(line)
            # print(parts)
            if parts[0].isdigit():
                # print(parts[0])
                #prevent division by 0
                if currentIp is not None and delays:

                    # Calculate and write the average delay for the previous hop
                    average_delay = sum(delays) / len(delays)
                    file.write(f"{currentIp}  {'average delay: '} {average_delay:.2f}\n")

                if '*' in parts[1]:
                    currentIp = None;
                if '(' in parts[1] and ')' in parts[1]:
                    # [start : end]
                    currentIp = parts[1][parts[1].find('(')+1 : parts[1].find(')')]
                    delays = []

                elif '(' in parts[2] and ')' in parts[2]:
                    currentIp = parts[2][parts[2].find('(')+1 : parts[2].find(')')]
                    delays = []
                # print(currentIp)


            # To prevent adding part[0] to our delays we have to start appending delays after parts[1]
             # Add up to 3 delay values from the rest of the parts
            for part in parts[1:]:
                if part.replace('.','',1).isdigit():
                    delay = float(part)
                    # Append each delay to delays array for current line
                    delays.append(delay)
        if currentIp is not None and delays:
            average_delay = sum(delays) / (len(delays))
            file.write(f"{currentIp}  {'average delay: '} {average_delay:.2f}\n")

def parsePingFile(pingImport, pingExport):
    with open(pingImport, 'r') as file:
        lines = file.readlines()
    # currentRTT = none
    delays = []

    with open (pingExport, 'w') as file:
        for line in lines:
            parts = line.strip().split()
            if not parts:
                continue
            # print(line)
            # print(parts)

            if parts[0].isdigit():
                if "time" in parts[6]:
                    time_string = parts[6]
                    splitfloat = time_string.split('=')
                    delay = float(splitfloat[1])
                    delays.append(delay)
                    print(delays)
            else:
                if "/" in parts[3]:
                    minstring = parts[3]
                    splitmin = minstring.split('/')
                    rttMin = float(splitmin[0])
                    # print(rttMin)
        sumdelay=0
        for delay in delays:
            sumdelay += delay-rttMin


        averageQueueingDelay = sumdelay/len(delays)
        print(averageQueueingDelay)
parseTracerouteFile('traceroutetest.txt', 'average_delays.txt')

parsePingFile('pingtest.txt', 'ping_delays.txt')