/*Copyright 2025 Kamlesh Singh
*
*Licensed under the Apache License, Version 2.0 (the "License");
*you may not use this file except in compliance with the License.
*You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
*Unless required by applicable law or agreed to in writing, software
*distributed under the License is distributed on an "AS IS" BASIS,
*WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*See the License for the specific language governing permissions and
*limitations under the License.
*/

#ifndef COMMON_FRAMEWORK_APPLICATION_STATE_HPP
#define COMMON_FRAMEWORK_APPLICATION_STATE_HPP

// Define common application states
enum class AppState {
    UNINITIALIZED,  // Initial state, before any setup
    INITIALIZING,   // During setup
    READY,          // Initialized and waiting to run
    RUNNING,        // Actively performing its duties
    PAUSED,         // Temporarily suspended
    ERROR,          // Entered upon critical error
    TERMINATING,    // During shutdown sequence
    TERMINATED      // Application fully shut down
};

// Define common application events that trigger state transitions
enum class AppEvent {
    INIT_REQUEST,      // External or internal request to initialize
    INIT_SUCCESS,      // Initialization completed successfully
    INIT_FAIL,         // Initialization failed
    START_REQUEST,     // Request to start running
    PAUSE_REQUEST,     // Request to pause
    RESUME_REQUEST,    // Request to resume from paused
    STOP_REQUEST,      // Request to stop gracefully
    ERROR_OCCURRED,    // An error occurred
    SHUTDOWN_REQUEST,  // General shutdown request (e.g., from OS signal)
    TICK               // Periodic tick event (for execute loop)
};

#endif // COMMON_FRAMEWORK_APPLICATION_STATE_HPP