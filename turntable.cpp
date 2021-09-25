/*
 * NAME:    turntable.cpp
 * AUTHOR:  Jake Rogers
 * VERSION: 1.0
 * 
 * USAGE: turntable filepath [-f frame_count] [-a] [-l] [-axis X | Y] [-v velocity]
 * 
 * Provides the necessary standard input to present a PolySet defined in the file at
 * 'filepath'. The presentation will be animated to spin the object on the 'Z' axis, as if
 * on a turntable.
 * 
 * -f frame_count: will cause the animation to end after a the specified number of frames.
 * -a: will render axis lines at the origin.
 * -l: will enable 3 axis-aligned far lights to better display angles, faces, and reflections.
 * -r X | Y : Changes the axis of rotation to the one specified.
 * -v velocity: Change the speed of the rotation
 * 
 * Feel free to edit the data in 'header' to change the camera properties for an even better view.
 * However, this will require recompilation in this version of the program.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <climits>

using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::stringstream;

bool use_axis_lights = false;
bool render_axes = false;
char axis = 'Z';
float velocity = 0.1;
unsigned int max_frames = UINT_MAX;

const char *header[] =
    {
        "Display  \"animation\" \"Screen\" \"rgbdouble\"",
        "Format 640 480",
        "CameraEye 8 8 -5",
        "CameraAt 0 0 0",
        "CameraUp 0 0 1",
        "CameraFOV 76"
    };

string poly_from_file(string path)
{
    stringstream ss;
    string line;

    ifstream in_stream;
    in_stream.open(path);
    if (in_stream.is_open())
    {
        while (in_stream.good())
        {
            getline(in_stream, line);
            ss << line << std::endl;
        }
    }
    else
    {
        std::cerr << "Failed to open polyfile at " << path << "!" << std::endl;
        return "";
    }

    return ss.str();
}

int n_header = sizeof(header) / sizeof(char *);

void create_axes()
{
    cout << "ObjectBegin \"Axes\"\n";
    cout << "Color 1 0 0\n";
    cout << "Line 0 0 0 10 0 0\n";
    cout << "Color 0 1 0\n";
    cout << "Line 0 0 0 0 10 0\n";
    cout << "Color 0 0 1\n";
    cout << "Line 0 0 0 0 0 10\n";
    cout << "ObjectEnd\n";
}

void usage()
{
    std::cerr << "Usage: turntable filepath" << endl;
    std::cerr << "\t-f specify maximum number of frames, default UINT_MAX" << endl;
    std::cerr << "\t-a render axis lines at the origin" << endl;
    std::cerr << "\t-l Enable 3 axis-aligned lights to better show off angles (Rather than ambient lighting)" << std::endl;
    std::cerr << "\t-r X | Y : Changes the axis of rotation to the one specified" << endl;
    std::cerr << "\t-v velocity: Change the speed of the rotation" << endl;
    exit(1);
}

void check_opts(int argc, char *argv[])
{
    for (int i = 0; i < argc; i++) {
        char* token = argv[i];
        if (token[0] == '-') {
            switch (token[1]) {
                case('f'):
                    if (i == argc - 1) {
                        std::cerr << "Must specify unsigned integer for -f!";
                        exit(-1);
                    }
                    max_frames = atoi(argv[i+1]);
                    if (max_frames < 1) {
                        std::cerr << "Invalid frame count!" << endl;
                        exit(-1);
                    }
                    break;
                
                case('a'):
                    render_axes = true;
                    break;

                case('l'):
                    use_axis_lights = true;
                    break;
                
                case('r'):
                    if (i == argc - 1) {
                        std::cerr << "Must specify either X or Y for -r";
                        exit(-1);
                    }
                    axis = *argv[i+1];
                    axis = toupper(axis);

                    if (axis != 'X' && axis != 'Y') {
                        std::cerr << "Invalid axis for -r!";
                        exit(-1);
                    }
                    break;
                
                case('v'):
                    if (i == argc - 1) {
                        std::cerr << "Must specify velocity for -v";
                        exit(-1);
                    }

                    velocity = atof(argv[i+1]);
                    break;

                default:
                    cout << "Invalid option \"" << token[1] << "\"" << endl;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        usage();
    }
    check_opts(argc, argv);
    string poly = poly_from_file(argv[1]);

    // Print header
    for (int i = 0; i < n_header; i++)
        cout << header[i] << endl;

    create_axes();
    // Animation loop here
    int frame = 0;
    while (frame != max_frames)
    {
        cout << "FrameBegin " << frame << endl;
        cout << "WorldBegin\n";

        if (use_axis_lights)
        {
            cout << "FarLight -1 0 0 1 0 0 0.5\n";
            cout << "FarLight 0 -1 0 0 1 0 0.5\n";
            cout << "FarLight 0 0 -1 0 0 1 0.5\n";
            cout << "AmbientLight 1 1 1 0.8\n";
        }
        else
        {
            cout << "AmbientLight 1 1 1 1\n";
        }

        cout << "Ka 0.5\n";
        cout << "Kd 1\n";

        if (render_axes)
            cout << "ObjectInstance \"Axes\"\n";

        cout << "Rotate \"" << axis << "\" " << velocity * frame << endl;
        cout << poly;
        cout << "WorldEnd\n";
        cout << "FrameEnd\n";

        frame++;
    }

    return 0;
}