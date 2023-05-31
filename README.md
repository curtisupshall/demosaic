# demosaic

https://en.wikipedia.org/wiki/Demosaicing

# Environment Setup:

## UVIC VPN:

### Requirements:

    - Cisco Anyconnect VPN Client
        - https://www.uvic.ca/systems/services/internettelephone/remoteaccess/
    - MobaXterm (Parent Terminal Controller)
        - https://mobaxterm.mobatek.net/
    - PuTTY (SSH Client)
        - https://www.putty.org/

### Procedure:

    - Connect to `vpn.uvic.ca`
        - Group: 3 - Student
        - 2FA

    - Open MobaXTerm
        - Left side panel, "Sessions" (Star)
        - Right click, New session
        - SSH

    - Connect to ECE Undergraduate Lab
        - Remote host: `$ ssh ugls.ece.uvic.ca`
        - [ ] Specify username: netlink ID
        - OK

    - Clone `this` Repo to the SSH PC

    - Compile code
        - Run compile_file.sh: `$ ./scripts/compile_file.sh`
            - note: might not have permission: `$ chmod +x ./scripts/compile_file.sh`

    - Connect to Seng 440 ECE PC
        - Refer above `seng440.ece.uvic.ca`

        - Run `$ lftp -u user1,q6coHjd7P @arm`

        - Run `$ put ./src/compile/basic.exe`

        - Exit (Or open another machine)

        - Telnet into Arm machine
            - `$ telnet arm`
                - `(user1, user2, user3, user4)`
            - `q6coHjd7P`
            - `chmod +x basic.exe`
            - `./basic.exe`
