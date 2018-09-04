// VTK include
#include <vtksys/CommandLineArguments.hxx>
#include <vtkNew.h>
#include <vtkOBJReader.h>
#include "vtkOBJWriter.h"
#include <vtkPLYReader.h>
#include <vtkPLYWriter.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataWriter.h>
#include <vtkSTLReader.h>
#include <vtkSTLWriter.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkXMLPolyDataWriter.h>

#include <algorithm> // std::transform
#include <iostream>

//-----------------------------------------------------------------------------
// FUNCTIONS
//-----------------------------------------------------------------------------
bool ReadArguments(int argc, char ** argv);
unsigned long ReadMesh(std::string path, vtkPolyData* mesh);
unsigned long WriteMesh(std::string path, vtkPolyData* mesh);
bool FindString(std::string researchString, std::string fullString);

//-----------------------------------------------------------------------------
// READING ARGUMENTS
//-----------------------------------------------------------------------------
std::string pathToInputFile;
std::string pathToOutputFile;

int main(int argc, char* argv[])
{
  if (!ReadArguments(argc, argv))
  {
    return EXIT_FAILURE;
  }

  vtkPolyData* meshWithNormals = vtkPolyData::New();
  int readErr = ReadMesh(pathToInputFile, meshWithNormals);
  if (readErr == -1)
  {
    return EXIT_FAILURE;
  }

  int writeErr = WriteMesh(pathToOutputFile, meshWithNormals);
  if (writeErr == -1)
  {
    return EXIT_FAILURE;
  }

  std::cout << "Written : " << pathToOutputFile << std::endl;

  return EXIT_SUCCESS;
}

//-----------------------------------------------------------------------------
/* Read input argument and check if they are valid */
bool ReadArguments(int argc, char ** argv)
{
  bool help = false;

  vtksys::CommandLineArguments arg;
  arg.Initialize(argc, argv);
  typedef vtksys::CommandLineArguments argT;

  arg.AddArgument("--input", argT::SPACE_ARGUMENT, &pathToInputFile, "(required) Path to the input file (.ply, .vtk, .vtp, .obj, .stl)");
  arg.AddArgument("--output", argT::SPACE_ARGUMENT, &pathToOutputFile, "(required) Path of the output file (.ply, .vtk .vtp, .obj, .stl)");
  arg.AddBooleanArgument("--help", &help, "Print help message");

  int result = arg.Parse();
  if (!result || help)
  {
    std::cerr << arg.GetHelp();
    return false;
  }

  // Check arguments
  if (pathToInputFile == "" || pathToOutputFile == "")
  {
    std::cerr << "Missing arguments..." << std::endl;
    std::cerr << arg.GetHelp();
    return false;
  }

  return true;
}

//-----------------------------------------------------------------------------
/* Read input file */
unsigned long ReadMesh(std::string path, vtkPolyData* mesh)
{
  vtkAlgorithm* genericReader = nullptr;

  if (FindString(".ply", path))
  {
    vtkPLYReader* reader = vtkPLYReader::New();
    reader->SetFileName(path.c_str());
    genericReader = reader;
  }
  else if (FindString(".vtk", path))
  {
    vtkPolyDataReader* reader = vtkPolyDataReader::New();
    reader->SetFileName(path.c_str());
    genericReader = reader;
  }
  else if (FindString(".vtp", path))
  {
    vtkXMLPolyDataReader* reader = vtkXMLPolyDataReader::New();
    reader->SetFileName(path.c_str());
    genericReader = reader;
  }
  else if (FindString(".obj", path))
  {
    vtkOBJReader* reader = vtkOBJReader::New();
    reader->SetFileName(path.c_str());
    genericReader = reader;
  }
  else if (FindString(".stl", path))
  {
    vtkSTLReader* reader = vtkSTLReader::New();
    reader->SetFileName(path.c_str());
    genericReader = reader;
  }

  if (genericReader == nullptr)
  {
    std::cerr << "No elligible reader for this type of file" << std::endl;
    return -1;
  }

  genericReader->Update();
  mesh->DeepCopy(vtkPolyData::SafeDownCast(genericReader->GetOutputDataObject(0)));

  genericReader->Delete();
  return 1;
}

//-----------------------------------------------------------------------------
/* Write output file */
unsigned long WriteMesh(std::string path, vtkPolyData * mesh)
{
  vtkAlgorithm* genericWriter = nullptr;

  if (FindString(".ply", path))
  {
    vtkPLYWriter* writer = vtkPLYWriter::New();
    writer->SetFileName(path.c_str());
    writer->SetInputData(mesh);
    genericWriter = writer;
  }
  else if (FindString(".vtk", path))
  {
    vtkPolyDataWriter* writer = vtkPolyDataWriter::New();
    writer->SetFileName(path.c_str());
    writer->SetInputData(mesh);
    genericWriter = writer;
  }
  else if (FindString(".vtp", path))
  {
    vtkXMLPolyDataWriter* writer = vtkXMLPolyDataWriter::New();
    writer->SetFileName(path.c_str());
    writer->SetInputData(mesh);
    genericWriter = writer;
  }
  else if (FindString(".obj", path))
  {
    vtkOBJWriter* writer = vtkOBJWriter::New();
    writer->SetFileName(path.c_str());
    writer->SetInputData(mesh);
    genericWriter = writer;
  }
  else if (FindString(".stl", path))
  {
    vtkSTLWriter* writer = vtkSTLWriter::New();
    writer->SetFileName(path.c_str());
    writer->SetInputData(mesh);
    genericWriter = writer;
  }

  if (genericWriter == nullptr)
  {
    std::cerr << "No elligible writer for this type of file" << std::endl;
    return -1;
  }

  genericWriter->Update();

  return 0;
}

//-----------------------------------------------------------------------------
/* Find 'researchString' inside 'fullString' with insensitive case */
bool FindString(std::string researchString, std::string fullString)
{
  std::string fs = fullString;
  std::transform(fs.begin(), fs.end(), fs.begin(), ::toupper);
  std::string rs = researchString;
  std::transform(rs.begin(), rs.end(), rs.begin(), ::toupper);

  if (fs.find(rs) != std::string::npos)
    return true;

  return false;
}
