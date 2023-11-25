import javax.swing.*;
import java.awt.event.*;
import java.io.*;

public class Project_main {
    public static void main(String[] args) {
        JFrame frame = new JFrame("CP317_project");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(960, 540);

        JSplitPane splitPane = new JSplitPane();
        frame.getContentPane().add(splitPane);

        JPanel leftPanel = new JPanel();
        leftPanel.setLayout(new BoxLayout(leftPanel, BoxLayout.Y_AXIS));
        splitPane.setLeftComponent(leftPanel);

        // File Input Buttons
        JButton button1 = createFileInputButton("Name File Input");
        leftPanel.add(button1);

        JButton button2 = createFileInputButton("Course File Input");
        leftPanel.add(button2);

        //JButton button3 = createFileInputButton("File Input 3");
        //leftPanel.add(button3);

        // Run C++ Program Button
        JButton runButton = new JButton("Run C++ Program");
        runButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                runCppProgram();
            }
        });
        leftPanel.add(runButton);

        // Check C++ Output Button
        JButton checkOutputButton = new JButton("Check output");
        leftPanel.add(checkOutputButton);

        JPanel rightPanel = new JPanel();
        rightPanel.setLayout(new BoxLayout(rightPanel, BoxLayout.Y_AXIS));
        JTextArea textArea = new JTextArea(10, 30);
        JScrollPane scrollPane = new JScrollPane(textArea);
        rightPanel.add(scrollPane);

        checkOutputButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                loadOutputToTextArea(textArea, "output.txt");
            }
        });

        // Exit Button
        JButton exitButton = new JButton("Exit");
        exitButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                clearFile("sharedFilePaths.txt");
                System.exit(0);
            }
        });
        rightPanel.add(exitButton);

        splitPane.setRightComponent(rightPanel);

        frame.setVisible(true);
    }

    private static JButton createFileInputButton(String buttonText) {
        JButton button = new JButton(buttonText);
        button.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                JFileChooser fileChooser = new JFileChooser();
                int result = fileChooser.showOpenDialog(null);
                if (result == JFileChooser.APPROVE_OPTION) {
                    File selectedFile = fileChooser.getSelectedFile();
                    writeFilePathToFile(selectedFile.getAbsolutePath());
                }
            }
        });
        return button;
    }

    private static void writeFilePathToFile(String filePath) {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter("sharedFilePaths.txt", true))) {
            writer.write(filePath + "\n");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static void clearFile(String filename) {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(filename, false))) {
            writer.write("");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static void runCppProgram() {
        try {
            String cppProgram = "CP317_project.exe"; 
            ProcessBuilder builder = new ProcessBuilder(cppProgram);
            builder.directory(new File(System.getProperty("user.dir"))); // Set the working directory to current directory
            builder.redirectErrorStream(true); // Redirect error stream to output stream
            Process process = builder.start();

            // Read output from the C++ program
            BufferedReader reader = new BufferedReader(new InputStreamReader(process.getInputStream()));
            String line;
            while ((line = reader.readLine()) != null) {
                System.out.println(line);
            }
            reader.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static void loadOutputToTextArea(JTextArea textArea, String filePath) {
        BufferedReader reader = null;
        try {
            reader = new BufferedReader(new FileReader(filePath));
            String line;
            textArea.setText(""); // we're just clearing the previous text
            while ((line = reader.readLine()) != null) {
                textArea.append(line + "\n");
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (reader != null) {
                try {
                    reader.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }
}
