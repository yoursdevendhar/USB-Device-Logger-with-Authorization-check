USB Device Access Logger with Authorization Check

Problem Statement:

Simulate a system that tracks USB device plug-ins and verifies whether the device is on the trusted list.

Objectives:

1. To simulate USB device connections
2. To check device ID against allowed list
3. To block unauthorized devices
4. To log all access events

Modules:

1. Device Entry Module - Inputs device ID and type
2. Trusted List Loader Stores authorized IDs
3. Access Validator Grants/blocks access
4. Alert Engine - Flags unknown devices
5. Access Log Exporter -Saves logs
