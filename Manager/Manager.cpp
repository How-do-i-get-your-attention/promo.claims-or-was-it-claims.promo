#include "Manager.h"

/// The Write function adds a message along with the current time and error flag to the Messages vector.
void Write(string context, bool Error) {
    // Check if the 'By' value is equal to 'BySetup'
    if (By == BySetup) {
        // Add the message tuple to the Messages vector
        Messages.push_back(Error);
        // Get the current time
        auto currentTime = std::chrono::system_clock::now();
        auto currentTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch()) % 1000;
        // Convert the current time to a time_t
        auto currentTimeT = std::chrono::system_clock::to_time_t(currentTime);
        // Convert the time_t to a local time struct tm
        std::tm localTime{};
        localtime_s(&localTime, &currentTimeT);
        // Format the local time as a string including milliseconds
        char timeBuffer[80];
        std::strftime(timeBuffer, sizeof(timeBuffer), "%d/%m/%Y %H:%M:%S", &localTime);
        sprintf_s(timeBuffer + 17, sizeof(timeBuffer) - 17, "%03lld", currentTimeMs.count());
        // Print the time, error status, and message to the console
        cout << "+------------------------+" << endl
            << "| Time: " << timeBuffer <<  endl
            << "| Error: " << (Error ? "Yes" : "No")  << endl
            << "| Message: " << context  << endl
            << "+------------------------+" << endl;
    }
}

bool Setup(path, path, path, HMODULE)
{
    By = BySetup;
    Write("hallo world write", true);
    Write("hallo world write", true);
    for (bool error : Messages)
        if (error)
            return false; // Return false if any message has an error flag set to false
    return true; // Return true if all messages have error flags set to true

}

void Services(path, path, HMODULE)
{
    By = ByServices;
}

void Goodbye()
{
}
