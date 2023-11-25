#include<stdio.h>
#include<fstream>
#include<iostream>
#include <string.h>
#include <ctype.h>
#include <conio.h> //for getch()

using namespace std;

/*
TODO:

file choosing/launch options file choosing

more extensive testing
warning/error messages for no file found etc
*/

/*
bugs/fixes:

empty name doesnt give error
*/


class file_to_read {
    protected:
    ifstream file;
    string line;
    int line_number;
    int length_in_lines;
    string first_three_lines;

    public:

    //constructor declaration
    file_to_read(string my_str);

    
    //print one line
    string next_line() {
        getline(file, line);
        line_number++;
        return line;
    }
    
    string return_line() {
        return line;
    }
    int return_line_number(){
        return line_number;
    }
    int return_lengh_in_lines(){
        return length_in_lines;
    }

    
    
    void go_to_start() {

        file.clear();
        file.seekg(0);
        getline(file, line);
        line_number = 0;
        return;
    }
    
    
    void go_to_line(int p) {
        //incase were past the desired line
        if (line_number > p) {
            go_to_start();
        } 
        
        //go forwards till were at the desired line
        for (int i = line_number; i < p; i++) {
            next_line();
        }
        return;
    }

    int check_seperator(int index) {
        if (line[index] == ',' && line[index+1] == ' ') {
            //printf("success");
            return 1;
        } else {
            //printf("failed last");
            return 0;
        }
    }
    
    
    //for testing
    void test_print_whole_file() {
        for( string line; getline( file, line ); ) {
            printf("%s\n", line.c_str());
        }
        return;
    }
    
    //for testing
    
    void test_print_current_status() {
        printf("current line: %s ||line# %d ||max lines %d\n", line.c_str(), line_number, length_in_lines);
        return;
    }
    

};

//constructor
file_to_read::file_to_read(string my_str) {
    
    file.open(my_str);
    if (file.fail()) {
        printf("failed to open %s, aborting\n", my_str.c_str());
    }

    line_number = 0;
    length_in_lines = 0;
    
    
    //counting lines
    
    for( string line2; getline( file, line2 ); ) {
        length_in_lines++;
        if (length_in_lines < 3) {
            first_three_lines.append(line2);
        }
    }
    
    file.clear();
    file.seekg(0);

    getline(file, line);


}

//make sure this all works
class name_file: public file_to_read {
    public:
    using file_to_read::file_to_read;
    
    int verify_line() {
        
        //making sure its atleast the right length
        
        if (line.length() <= 11) {
            //printf("failed length");
            return 0;
        }
        
        //checking the student # is only digits 0-9
        for (int i=0; i<9; i++) {
            if (!isdigit(line[i])) {
                //printf("failed digit");
                return 0;    
            }
        }
        
        //checking , and space seperator
        //return if successful. not checking names since names can be weird.
        if (!(line[9] == ',' && line[10] == ' ')) {
            return 0;
        }

        //making sure name is only alpha and spaces
        for (int i = 11; i<line.length(); i++) {
            if ((!isalpha(line[i])) && line[i] != ' ' ){
                return 0;
            }
        }

        return 1;
    }
    
    
    //verifys the whole file, since ids must be unique
    int verify_file() {
        int remembered_line_number;
        string remembered_id;
        int valid;

        //making sure were at the start
        go_to_start();
        
        while (line_number < length_in_lines) {
            //making sure all lines are valid
            valid = verify_line();
            if (valid == 0) {
                //printf("an invalid line");
                return 0;
            }
            next_line();
        }
        
        //resetting
        go_to_start();
        
        //checking for uniqueness
        remembered_line_number = 0;
        while (remembered_line_number < length_in_lines){
            //set what were going to check against everything else
            remembered_line_number = line_number;
            remembered_id = line.substr(0, 9);

            //printf("\n remembered_line_number: %d , remembered_id %s , line_number %d, line id %s", remembered_line_number , remembered_id.c_str(), line_number, return_line().substr(0,9).c_str());

                        
            //move 1 further so we dont check ourself
            next_line();

            while (line_number < length_in_lines) {
                
                if (remembered_id == return_line().substr(0,9)) {
                    //printf("invalid uniqueness");
                    return 0; //invalid, duplicate ids
                } else {
                    //printf("doing next line");
                    next_line();
                }

                //printf("\n remembered_line_number: %d , remembered_id %s , line_number %d, line id %s", remembered_line_number , remembered_id.c_str(), line_number, return_line().substr(0,9).c_str());


            }
            remembered_line_number = remembered_line_number+1;
            go_to_line(remembered_line_number);
            //getch(); 
        }
        
        //resetting before we leave
        go_to_start();
        return 1; //all valid since we got to the end without returning
    
    }
    
};



//make sure this all works
class grade_file: public file_to_read {
    public:
    using file_to_read::file_to_read;


    float calculate_final_grade() {
        
        //getting all the numbers into nice little strings
        string working_string;
        string grade1;
        string grade2;
        string grade3;
        string grade4;

        working_string = line.substr(18, line.length()-18);

        grade1 = working_string.substr(0, working_string.find(", ")); //extracting grade
        working_string = working_string.substr(working_string.find(", ")+2, working_string.length()-working_string.find(", ")+2); //removing what we just took
        

        grade2 = working_string.substr(0, working_string.find(", ")); //extracting grade
        working_string = working_string.substr(working_string.find(", ")+2, working_string.length()-working_string.find(", ")+2); //removing what we just took
        
        grade3 = working_string.substr(0, working_string.find(", ")); //extracting grade
        working_string = working_string.substr(working_string.find(", ")+2, working_string.length()-working_string.find(", ")+2); //removing what we just took
        
        grade4 = working_string.substr(0, working_string.find(", ")); //extracting grade
        working_string = working_string.substr(working_string.find(", ")+2, working_string.length()-working_string.find(", ")+2); //removing what we just took
        
        //calcing the final grade
        float final_grade = stof(grade1) * 0.2 + stof(grade2) * 0.2 + stof(grade3) * 0.2 + stof(grade4) * 0.4;
        
        return final_grade;
    }

    

    
    int verify_line() {

        
        
        //quick length check
        if (line.length() <= 17) {
            //printf("failed length");
            return 0;
        }
        
        //checking the student # is only digits 0-9
        for (int i=0; i<9; i++) {
            if (!isdigit(line[i])) {
                //printf("failed digit");
                return 0;    
            }
        }

        if (!check_seperator(9)) {
            return 0;
        }

        //checking course codes valid, must be 'AA###'
        if ( !isalpha(line[11]) ||
            !isalpha(line[12]) ||
            !isdigit(line[13]) ||
            !isdigit(line[14]) ||
            !isdigit(line[15])) {
                return 0;
            }
        
        if (!check_seperator(16)) {
            return 0;
        }
        
        //grade stuff
        //This is so awful and I hate it, but i really dont wanna work with pointers to make a function that will do this nicely.
        string working_string;
        int find_result;
        string grade1;
        string grade2;
        string grade3;
        string grade4;

        working_string = line.substr(18, line.length()-18);

        grade1 = working_string.substr(0, working_string.find(", ")); //extracting grade
        if (grade1.length() == 0) return 0; //empty = no good
        find_result = working_string.find(", ");
        if (find_result == -1) return 0; //returning error if seperator wasnt found
        working_string = working_string.substr(find_result+2, working_string.length()-working_string.find(", ")+2); //removing what we just took
        for (int i=0; i<grade1.length(); i++){
            if (!isdigit(grade1[i])){
                return 0;
            }
        }

        grade2 = working_string.substr(0, working_string.find(", ")); //extracting grade
        if (grade2.length() == 0) return 0; //empty = no good
        find_result = working_string.find(", ");
        if (find_result == -1) return 0; //returning error if seperator wasnt found
        working_string = working_string.substr(find_result+2, working_string.length()-working_string.find(", ")+2); //removing what we just took
        for (int i=0; i<grade2.length(); i++){
            if (!isdigit(grade2[i])){
                return 0;
            }
        }

        grade3 = working_string.substr(0, working_string.find(", ")); //extracting grade
        if (grade3.length() == 0) return 0; //empty = no good
        find_result = working_string.find(", ");
        if (find_result == -1) return 0; //returning error if seperator wasnt found
        working_string = working_string.substr(find_result+2, working_string.length()-working_string.find(", ")+2); //removing what we just took
        for (int i=0; i<grade3.length(); i++){
            if (!isdigit(grade3[i])){
                return 0;
            }
        }

        grade4 = working_string.substr(0, working_string.find(", ")); //extracting grade
        if (grade4.length() == 0) return 0; //empty = no good
        find_result = working_string.find(", ");
        //no checking for a lack of seperator here
        working_string = working_string.substr(find_result+2, working_string.length()-working_string.find(", ")+2); //removing what we just took
        for (int i=0; i<grade4.length(); i++){
            if (!isdigit(grade4[i])){
                return 0;
            }
        }


        return 1;
        
        
    }
    
    
    //verifys the whole file, since ids must be unique
    int verify_file() {
        
        //making sure were at the start
        go_to_start();
        int valid;

        while (line_number < length_in_lines) {
            //making sure all lines are valid
            valid = verify_line();
            if (valid == 0) {
                //printf("an invalid line");
                return 0;
            }
            next_line();
        }

        //resetting before we leave
        go_to_start();
        return 1; //all valid since we got to the end without returning
    
    }
    
};


class write_file {
    private:
    ofstream file;
    
    
    public:
    write_file(string my_str);
    void write_line(string str) {
        file << str+"\n";
        return;
    }

};

write_file::write_file(string my_str) {
    
    file.open(my_str);

}




/*-------------------------------------*/
//utility functions
/*-------------------------------------*/

int check_matching_ids(string line1, string line2) {
    if (line1.substr(0, 9) == line2.substr(0, 9)) {
        return 1;
    }
    return 0;
}





int main(int argc, char *argv[]) {


    string name_file_location = "";
    string grade_file_location = "";
    if (argc == 2 || argc > 3) {
        printf("Please either use no command arguments to default to the files namefile.txt and coursefile.txt \nor 2 extra arguments in the format 'cp317_project name_file_location grade_file_location'");
        name_file_location = "namefile.txt";
        grade_file_location = "coursefile.txt";
    } else if (argc == 3) {
        //yOuR sTrInGs WeReNt DeClArEd In ThIs ScOpE
        name_file_location.append(argv[1]);
        grade_file_location.append(argv[2]);
    } else {
        name_file_location = "namefile.txt";
        grade_file_location = "coursefile.txt";
    }
    
    /*printf("%s", argv[0]);
    printf("%s", argv[1]);
    printf("%s", argv[2]);*/


    name_file file1(name_file_location);
    grade_file file2(grade_file_location);
    write_file file3("output.txt");

    //verify the files
    if (file1.verify_file() == 0) {
        printf("Your name file input is formatted improperly\n");
        return 0;
    }
    if (file2.verify_file() == 0) {
        printf("Your course file input is formatted improperly\n");
        return 0;
    }
    

    
    //its kind of cringe that this whole thing just sits in main, but when i was doing a utility function everything would just break
    //a billion errors about deleted functions and what not.
    //goes through both read files, determines if they should match, then calls the file_to_write class to write.
    string name_line;
    string grade_line;
    string result_string;
    char * grade_string;
    float calculated_grade;

    while (file1.return_line_number() < file1.return_lengh_in_lines()) { //looping through name file
        while (file2.return_line_number() < file2.return_lengh_in_lines()) { //looping through grade file (resetting for each name file)
            if (check_matching_ids(file1.return_line(), file2.return_line())){

                //setting lines
                name_line = file1.return_line();
                grade_line = file2.return_line();

                //putting together the lines
                result_string = name_line;
                result_string.append(grade_line.substr(9, 9));

                calculated_grade = file2.calculate_final_grade();
                sprintf(grade_string, "%.1f", calculated_grade);
                result_string.append(grade_string);

                //printf("%s\n", result_string.c_str()); //remove this at the end maybe
                file3.write_line(result_string);
            }
            file2.next_line();
        }
        file1.next_line();
        file2.go_to_start();
    }
    printf("Successfully created the output file\n");
    return 0;
}