#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_DEVICES 100
#define MAX_ID_LENGTH 20
#define MAX_TYPE_LENGTH 15
#define MAX_LOG_ENTRIES 1000
#define LOG_FILE "usb_access_log.txt"

// Structure for USB device
typedef struct {
    char device_id[MAX_ID_LENGTH];
    char device_type[MAX_TYPE_LENGTH];
}USBDevice;

// Structure for log entry
typedef struct {
    char device_id[MAX_ID_LENGTH];
    char device_type[MAX_TYPE_LENGTH];
    char timestamp[30];
    char status[20]; // "AUTHORIZED" or "BLOCKED"
    char action[50];
} LogEntry;

// Global variables
USBDevice trusted_devices[MAX_DEVICES];
LogEntry access_logs[MAX_LOG_ENTRIES];
int trusted_count = 0;
int log_count = 0;

// Function prototypes
void initialize_trusted_list();
int device_entry(USBDevice *device);
int access_validator(const USBDevice *device);
void alert_engine(const USBDevice *device);
void log_access_event(const USBDevice *device, const char *status, const char *action);
void access_log_exporter();
void display_menu();
void display_trusted_devices();
void display_access_logs();
char* get_current_timestamp();

// Module 1: Device Entry Module
int device_entry(USBDevice *device) {
    printf("\n=== USB Device Connection Detected ===\n");
    printf("Enter Device ID: ");
    if (scanf("%19s", device->device_id) != 1) {
        printf("Error reading device ID\n");
        return 0;
    }
    
    printf("Enter Device Type: ");
    if (scanf("%14s", device->device_type) != 1) {
        printf("Error reading device type\n");
        return 0;
    }
    
    printf("Device registered: ID=%s, Type=%s\n", device->device_id, device->device_type);
    return 1;
}

// Module 2: Trusted List Loader
void initialize_trusted_list() {
    // Pre-load some trusted devices
    strcpy(trusted_devices[0].device_id, "USB001");
    strcpy(trusted_devices[0].device_type, "Mouse");
    
    strcpy(trusted_devices[1].device_id, "USB002");
    strcpy(trusted_devices[1].device_type, "Keyboard");
    
    strcpy(trusted_devices[2].device_id, "USB003");
    strcpy(trusted_devices[2].device_type, "Printer");
    
    strcpy(trusted_devices[3].device_id, "USB004");
    strcpy(trusted_devices[3].device_type, "Storage");
    
    strcpy(trusted_devices[4].device_id, "Dev");
    strcpy(trusted_devices[4].device_type, "Storage");
    
    trusted_count = 5;
    printf("Trusted device list loaded with %d devices.\n", trusted_count);
}

// Module 3: Access Validator
int access_validator(const USBDevice *device) {
    int i;
    printf("\n=== Validating Device Access ===\n");
    printf("Checking device ID: %s\n", device->device_id);
    
    for (i = 0; i < trusted_count; i++) {
        if (strcmp(trusted_devices[i].device_id, device->device_id) == 0) {
            printf("? Device AUTHORIZED: %s (%s)\n", device->device_id, device->device_type);
            log_access_event(device, "AUTHORIZED", "Access granted");
            return 1; // Authorized
        }
    }
    
    printf("? Device BLOCKED: %s (%s) - Not in trusted list\n", device->device_id, device->device_type);
    log_access_event(device, "BLOCKED", "Access denied - unauthorized device");
    return 0; // Blocked
}

// Module 4: Alert Engine
void alert_engine(const USBDevice *device) {
    printf("\n?? SECURITY ALERT ??\n");
    printf("Unauthorized USB device detected!\n");
    printf("Device ID: %s\n", device->device_id);
    printf("Device Type: %s\n", device->device_type);
    printf("Action: Connection blocked\n");
    printf("Timestamp: %s\n", get_current_timestamp());
    printf("Please contact system administrator.\n");
}

// Helper function to get current timestamp
char* get_current_timestamp() {
    static char timestamp[30];
    time_t now = time(0);
    struct tm *local_time = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", local_time);
    return timestamp;
}

// Log access events
void log_access_event(const USBDevice *device, const char *status, const char *action) {
    if (log_count < MAX_LOG_ENTRIES) {
        strcpy(access_logs[log_count].device_id, device->device_id);
        strcpy(access_logs[log_count].device_type, device->device_type);
        strcpy(access_logs[log_count].timestamp, get_current_timestamp());
        strcpy(access_logs[log_count].status, status);
        strcpy(access_logs[log_count].action, action);
        log_count++;
    }
}

// Module 5: Access Log Exporter
void access_log_exporter() {
     int i;
    FILE *file = fopen(LOG_FILE, "w");
    if (file == NULL) {
        printf("Error: Cannot create log file\n");
        return;
    }
    
    fprintf(file, "=== USB Device Access Log ===\n");
    fprintf(file, "Generated: %s\n\n", get_current_timestamp());
    fprintf(file, "%-12s %-12s %-20s %-12s %s\n", "Device ID", "Type", "Timestamp", "Status", "Action");
    fprintf(file, "================================================================================\n");
    
    for (i = 0; i < log_count; i++) {
        fprintf(file, "%-12s %-12s %-20s %-12s %s\n",
                access_logs[i].device_id,
                access_logs[i].device_type,
                access_logs[i].timestamp,
                access_logs[i].status,
                access_logs[i].action);
    }
    
    fclose(file);
    printf("Access log exported to %s\n", LOG_FILE);
    printf("Total entries logged: %d\n", log_count);
}

// Display functions
void display_menu() {
    printf("\n=== USB Device Access Logger ===\n");
    printf("1. Simulate USB Device Connection\n");
    printf("2. View Trusted Devices\n");
    printf("3. View Access Logs\n");
    printf("4. Export Logs to File\n");
    printf("5. Exit\n");
    printf("Enter your choice: ");
}

void display_trusted_devices() {
     int i;
    printf("\n=== Trusted Device List ===\n");
    printf("%-12s %s\n", "Device ID", "Device Type");
    printf("==========================\n");
    for (i = 0; i < trusted_count; i++) {
        printf("%-12s %s\n", trusted_devices[i].device_id, trusted_devices[i].device_type);
    }
}

void display_access_logs() {
     int i;
    printf("\n=== Access Log History ===\n");
    if (log_count == 0) {
        printf("No access events logged yet.\n");
        return;
    }
    
    printf("%-12s %-12s %-20s %-12s %s\n", "Device ID", "Type", "Timestamp", "Status", "Action");
    printf("================================================================================\n");
    for (i = 0; i < log_count; i++) {
        printf("%-12s %-12s %-20s %-12s %s\n",
                access_logs[i].device_id,
                access_logs[i].device_type,
                access_logs[i].timestamp,
                access_logs[i].status,
                access_logs[i].action);
    }
}

// Main function
int main() {
    int choice;
    USBDevice current_device;
    
    printf("Initializing USB Device Access Logger...\n");
    initialize_trusted_list();
    
    while (1) {
        display_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }
        
        switch (choice) {
            case 1:
                if (device_entry(&current_device)) {
                    if (!access_validator(&current_device)) {
                        alert_engine(&current_device);
                    }
                }
                break;
                
            case 2:
                display_trusted_devices();
                break;
                
            case 3:
                display_access_logs();
                break;
                
            case 4:
                access_log_exporter();
                break;
                
            case 5:
                printf("Exiting USB Device Access Logger...\n");
                printf("Final log count: %d entries\n", log_count);
                return 0;
                
            default:
                printf("Invalid choice. Please enter 1-5.\n");
        }
    }
    
    return 0;
}