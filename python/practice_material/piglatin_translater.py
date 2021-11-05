# python piglatin_translater.py English.txt
# This program takes in a english text file with no blank lines and translates it to Piglatin
# improvements: vowels, -  yay and do not bring it to the end
# periods and commas
# modifies textfile instead of creating: fills it out
from sys import argv


# sentence: line from input file 
# returns: translated line
# descriptions: splits words, and concatenates to make translated line
# calls: makeword(w) : does translating 
def translate(sentence):
    words = sentence.split(" ")
    plwords = ""
    for w in words:
        plwords =  plwords + makeword(w)  
    return  plwords +"\n"

# w: word for translate(sentence) after split
# returns: translated word     
# descriptions: store first letter : copy rest - add ay and a space
# called by:  translate(sentence)
def makeword(w):
    fl = w[0]
    if "," in w:
       w = w.replace(",", "\n") 
    n_w = ""
    for c in w:
        if c != fl:
              n_w = n_w  + c    
    n_w = n_w + "-"+ fl + "ay "
    n_w = n_w.capitalize()
    return n_w

# purpose: stores file content into a string called contents
# input: filename - path found file(filename)
# output: string contents : eveything within file
# description: open(path_to_file ) - f.read()
# called by: file(filename)
def open_and_store_file(path_to_file ):
    open(path_to_file, 'r')
    with open( path_to_file ) as f:
        contents = f.read()
    return contents

#final is string - all I want in txtfile
# open()-  'w'  = right to and create if does not exist 
# create and puts string into the file
def file_output(final):
    f = open("Piglatin_translation.txt", 'w')
    f.write(final)

def main():
    con =  open_and_store_file(argv[1])
    content = con.split("\n")
    final =  con +"\n\nOuput File:\n"
    for l in content:
        pig_latin = translate(l)
        final = final + pig_latin

    file_output(final)
    #print(final)    




if __name__ == "__main__":
    main()   