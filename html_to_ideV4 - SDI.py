"""
Python HTML to header file parser
Version: V4 - modified for TLS_G3_ETH_SDI
"""
with open("website.h", 'w', encoding="utf8") as output_file:
    #output_file.write('#include <avr/pgmspace.h>\n') PROGMEM
    #output_file.write('const char website[] = ')
    with open('index.html', 'r', encoding="utf8") as input_file:
        output_file.write('void website(EthernetClient client, Switcher &sw1, Switcher &sw2, Switcher &sw3, const char* status1, const char* status2, const char* save, const char* version){')
        output_file.write('\n')  
        output_file.write('     client.print("<!DOCTYPE html>");\n')
        for line in input_file.readlines():      
            if line.find("//") != -1 and (line[line.find("//")-1] == "	" or line[line.find("//")-1] == " "): #remove "//" type of comments
                #print(line[line.find("//")-1])
                continue
            if line.strip():            
                output_file.write('     client.print(F("')                           
                line = line.replace('<!DOCTYPE html>', '')
                line = line.replace('	', '')                
                line = line.replace('\n', '')          
                line = line.replace('"', '\\"')
                line = line.replace('~', '"));\n     client.print(')
                line = line.replace('`', ');\n     client.print(".");\n     client.print(')
                line = line.replace('^', ');\n     client.print(F("')
                output_file.write(line) 
                output_file.write('"));')
                output_file.write('\n')
        output_file.write('}')
        #~a`b`c`d^
