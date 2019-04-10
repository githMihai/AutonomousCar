#include "filehandler.h"

FileHandler::FileHandler()
{

}

FileHandler::FileHandler(const char *fileName)
{
    this->outFile.open(fileName);
}

void FileHandler::write(const char *message)
{
    this->lock.lock();
    this->outFile << message;
    this->lock.unlock();
}

void FileHandler::close()
{
    this->outFile.close();
}
