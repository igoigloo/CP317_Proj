import javax.swing.*;
import java.awt.event.*;
import java.io.*;

public class Project_main {
    public static void main(String[] args) {
        //main window (frame) for the application
        JFrame frame = new JFrame("CP317_project");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE); // Make sure the program closes when we close the window
        frame.setSize(960, 540); // Setting the size of the window

        //creating a split pane to divide the window into two parts
        JSplitPane splitPane = new JSplitPane();
        frame.getContentPane().add(splitPane);

        //setting up the left panel with buttons
        JPanel leftPanel = new JPanel();
        leftPanel.setLayout(new BoxLayout(leftPanel, BoxLayout.Y_AXIS)); // Making the buttons align vertically
        splitPane.setLeftComponent(leftPanel); // Adding the panel to the left side of the split pane

        //creating buttons for file input
        JButton button1 = createFileInputButton("Name File Input"); 
        leftPanel.add(button1); //adding the button to the left panel

        JButton button2 = createFileInputButton("Course File Input"); 
        leftPanel.add(button2); //adding this button too

        //setting up the right panel to display text
        JPanel rightPanel = new JPanel();
        rightPanel.setLayout(new BoxLayout(rightPanel, BoxLayout.Y_AXIS)); 
        JTextArea textArea = new JTextArea(10, 30); //this is where we'll show text
        JScrollPane scrollPane = new JScrollPane(textArea); //making the text area scrollable
        rightPanel.add(scrollPane); 

        // check output button to view the C++ program's output
        JButton checkOutputButton = new JButton("Check output");
        checkOutputButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                loadOutputToTextArea(textArea, "output.txt"); // Load the output from a file to the text area
            }
        });

        // Exit button to close the application
        JButton exitButton = new JButton("Exit");
        exitButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                clearFile("sharedFilePaths.txt"); // Clear the file that stores the paths before exiting
                System.exit(0); // This line actually exits the program
            }
        });
        rightPanel.add(exitButton); // Adding the exit button to the right panel

        splitPane.setRightComponent(rightPanel); // Adding the right panel to the split pane

        // Run C++ Program button
        JButton runButton = new JButton("Run C++ Program");
        runButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                runCppProgram(textArea); // When clicked, this runs the C++ program and shows output in the text area
            }
        });
        leftPanel.add(runButton); // Adding the run button to the left panel
        leftPanel.add(checkOutputButton); // Adding the check output button next to it

        frame.setVisible(true); // Finally, make our window visible
    }

    // This method creates a button that opens a file chooser when clicked
    private static JButton createFileInputButton(String buttonText) {
        JButton button = new JButton(buttonText);
        button.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                // System.out.println("Opening file chooser..."); // Debug print
                JFileChooser fileChooser = new JFileChooser(); 
                int result = fileChooser.showOpenDialog(null); 
                if (result == JFileChooser.APPROVE_OPTION) {
                    File selectedFile = fileChooser.getSelectedFile(); // Get the file the user picked
                    writeFilePathToFile(selectedFile.getAbsolutePath()); 
                }
            }
        });
        return button; //returning the button we just created
    }

    // This method writes the path of the chosen file to another file called sharedfilepaths
    private static void writeFilePathToFile(String filePath) {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter("sharedFilePaths.txt", true))) {
            writer.write(filePath + "\n"); // Write the file path and go to the next line
        } catch (IOException e) {
            e.printStackTrace(); 
        }
    }

    //this method clears the contents of a file
    private static void clearFile(String filename) {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(filename, false))) {
            writer.write(""); //clears the file
        } catch (IOException e) {
            e.printStackTrace(); //print error if something goes wrong
        }
    }

    // This method runs the C++ program and shows its output in the text area
    private static void runCppProgram(JTextArea textArea) {
        try {
            String cppProgram = "CP317_project.exe"; 
            ProcessBuilder builder = new ProcessBuilder(cppProgram); //processbuilder can run external programs
            builder.directory(new File(System.getProperty("user.dir"))); 
            builder.redirectErrorStream(true); //to see errors in the same place as regular output
            Process process = builder.start(); // building the C++ program

            BufferedReader reader = new BufferedReader(new InputStreamReader(process.getInputStream()));
            String line;
            textArea.setText(""); //cleans the text area before showing new output
            while ((line = reader.readLine()) != null) {
                String finalLine = line; 
                // System.out.println("C++ Program Output: " + finalLine); // Debug print
                SwingUtilities.invokeLater(() -> textArea.append(finalLine + "\n")); // Adding each line of output to the text area
            }
            reader.close(); // Don't forget to close the reader
        } catch (IOException e) {
            e.printStackTrace(); // Print error if there's a problem running the program
        }
    }

    // This method loads the content of a file to the text area
    private static void loadOutputToTextArea(JTextArea textArea, String filePath) {
        BufferedReader reader = null;
        try {
            reader = new BufferedReader(new FileReader(filePath)); // FileReader reads text from files
            String line;
            textArea.setText(""); // Clearing existing text
            while ((line = reader.readLine()) != null) {
                textArea.append(line + "\n"); // Adding each line of the file to the text area
            }
        } catch (IOException e) {
            e.printStackTrace(); // Print an error message if we can't read the file
        } finally {
            if (reader != null) {
                try {
                    reader.close(); // Always close your files
                } catch (IOException e) {
                    e.printStackTrace(); // Print error if closing the file fails
                }
            }
        }
    }
}