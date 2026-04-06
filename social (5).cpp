#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime> // 🔽 ADDED
using namespace std;

// 🔽 ADDED: Message Class
class Message {
public:
    int senderId;
    int receiverId;
    string content;
    string timestamp;

    Message(int s, int r, string c) {
        senderId = s;
        receiverId = r;
        content = c;

        time_t now = time(0);
        timestamp = ctime(&now);
    }

    void display(string senderName) {
        cout << "From: " << senderName;
        cout << "Time: " << timestamp;
        cout << "Message: " << content << endl;
        cout << "----------------------\n";
    }
};
// 🔼 END ADDED

class Validator {
public:
    static bool isValidUsername(string username) {
        return username.length() >= 3;
    }

    static bool isValidEmail(string email) {
        return email.find('@') != string::npos &&
               email.find('.') != string::npos;
    }

    static bool isValidPassword(string password) {
        return password.length() >= 6;
    }
};

class User {
    int userId;
    string username;
    string password;
    string email;

    vector<int> friends;
    int postCount = 0;
    int likeCount = 0;
    int commentCount = 0;

public:
    User(int id, string u, string p, string e) {
        userId = id;
        username = u;
        password = p;
        email = e;
    }

    string getUsername() const { return username; }
    string getPassword() const { return password; }
    string getEmail() const { return email; }

    void addFriend(int id) { friends.push_back(id); }
    void removeFriend(int id) {
        friends.erase(remove(friends.begin(), friends.end(), id), friends.end());
    }
    int getFriendCount() const { return friends.size(); }

    void addPost() { postCount++; }
    void removePost() { if (postCount > 0) postCount--; }
    int getPostCount() const { return postCount; }

    void addLike() { likeCount++; }
    int getLikeCount() const { return likeCount; }

    void addComment() { commentCount++; }
    int getCommentCount() const { return commentCount; }
};

class Post {
private:
    int postId;
    string content;
    int authorId;
    string authorName;
    int likes = 0;
    vector<string> comments;

public:
    Post(int id, string text, int uId, string uName) {
        postId = id;
        content = text;
        authorId = uId;
        authorName = uName;
    }

    void displayPost() {
        cout << "Post ID: " << postId << endl;
        cout << "Author: " << authorName << endl;
        cout << "Content: " << content << endl;
        cout << "Likes: " << likes << " | Comments: " << comments.size() << endl;
        cout << "----------------------\n";
    }

    int getPostId() const { return postId; }
    int getAuthorId() const { return authorId; }

    void addLike() { likes++; }
    void addComment(string comment) { comments.push_back(comment); }
};

class welcome {
public:
    void display() {
        cout << "\n========================================\n";
        cout << "   WELCOME TO SOCIAL MEDIA SYSTEM\n";
        cout << "========================================\n";
        cout << "     Connect with your friends\n\n";
    }

    int loginOptions() {
        int choice;
        cout << "=========================\n";
        cout << "1. Login\n";
        cout << "2. New Registration\n";
        cout << "3. Exit\n";
        cout << "=========================\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cout << endl;
        return choice;
    }

    int userMenu() {
        int choice;
        cout << "\n------------- USER MENU -------------\n";
        cout << "1. View News Feed\n";
        cout << "2. Create a New Post\n";
        cout << "3. View My Posts\n";
        cout << "4. View a Post (by ID)\n";
        cout << "5. Like a Post\n";
        cout << "6. Comment on a Post\n";
        cout << "7. Delete My Post\n";
        cout << "8. View My Profile\n";
        cout << "9. Add a Friend\n";
        cout << "10. Delete Account\n";
        cout << "11. Send Message\n";        // 🔽 ADDED
        cout << "12. View Messages\n";       // 🔽 ADDED
        cout << "13. Logout\n";              // 🔽 ADDED
        cout << "-------------------------------------\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cout << endl;
        return choice;
    }
};

class SocialMediaSystem {
private:
    vector<Post> posts;
    vector<User> users;

    // 🔽 ADDED
    vector<Message> messages;
    // 🔼 END ADDED

    int nextUserId = 1;
    int nextPostId = 1;

public:
    bool usernameExists(string username) {
        for (User &user : users) {
            if (user.getUsername() == username) return true;
        }
        return false;
    }

    void registerUser() {
        string username, email, password;

        cout << "\n----- REGISTRATION -----\n";

        while (true) {
            cout << "Enter username: ";
            cin >> username;
            if (!Validator::isValidUsername(username))
                cout << "Username must be at least 3 characters\n\n";
            else if (usernameExists(username))
                cout << "Username already exists\n\n";
            else
                break;
        }

        while (true) {
            cout << "Enter email: ";
            cin >> email;
            if (!Validator::isValidEmail(email))
                cout << "Invalid email\n\n";
            else
                break;
        }

        while (true) {
            cout << "Enter password: ";
            cin >> password;
            if (!Validator::isValidPassword(password))
                cout << "Password must be at least 6 characters\n\n";
            else
                break;
        }

        users.push_back(User(nextUserId++, username, password, email));
        cout << "\nRegistration successful!\n\n";
    }

    int loginUser() {
        string username, password;
        cout << "\n----- LOGIN -----\n";
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        for (int i = 0; i < users.size(); i++) {
            if (users[i].getUsername() == username &&
                users[i].getPassword() == password) {
                cout << "\nLogin successful! Welcome " << username << "\n\n";
                return i;
            }
        }
        cout << "\nInvalid username or password.\n\n";
        return -1;
    }

    // 🔽 ADDED: Send Message
    void sendMessage(int userIndex) {
        string receiverUsername, content;
        cout << "Enter receiver username: ";
        cin >> receiverUsername;

        int receiverIndex = -1;
        for (int i = 0; i < users.size(); i++) {
            if (users[i].getUsername() == receiverUsername) {
                receiverIndex = i;
                break;
            }
        }

        if (receiverIndex == -1) {
            cout << "User not found!\n\n";
            return;
        }

        cin.ignore();
        cout << "Enter message: ";
        getline(cin, content);

        messages.push_back(Message(userIndex, receiverIndex, content));

        cout << "Message sent!\n\n";
    }
    

    
    void viewMessages(int userIndex) {
        bool found = false;
        cout << "\n----- YOUR MESSAGES -----\n";

        for (Message &m : messages) {
            if (m.receiverId == userIndex) {
                m.display(users[m.senderId].getUsername());
                found = true;
            }
        }

        if (!found) cout << "No messages found.\n";
        cout << endl;
    }
    // 🔼 END ADDED

    // (rest of your functions unchanged...)
    void createPost(int userIndex) { /* SAME */ }
    void viewNewsFeed() { /* SAME */ }
    void commentOnPost(int userIndex) { /* SAME */ }
    void viewMyPosts(int userIndex) { /* SAME */ }
    void deleteMyPost(int userIndex) { /* SAME */ }
    void viewProfile(int userIndex) { /* SAME */ }
    void addFriend(int userIndex) { /* SAME */ }
    void deleteAccount(int &userIndex) { /* SAME */ }
    void likePost(int userIndex) { /* SAME */ }
    void viewPostById() { /* SAME */ }
};

int main() {
    SocialMediaSystem system;
    welcome w;

    while (true) {
        w.display();
        int choice = w.loginOptions();

        if (choice == 1) {
            int currentUser = system.loginUser();
            if (currentUser != -1) {
                int userChoice;
                do {
                    userChoice = w.userMenu();

                    if (userChoice == 1) system.viewNewsFeed();
                    else if (userChoice == 2) system.createPost(currentUser);
                    else if (userChoice == 3) system.viewMyPosts(currentUser);
                    else if(userChoice == 4) system.viewPostById();
                    else if (userChoice == 5) system.likePost(currentUser);
                    else if (userChoice == 6) system.commentOnPost(currentUser);
                    else if (userChoice == 7) system.deleteMyPost(currentUser);
                    else if (userChoice == 8) system.viewProfile(currentUser);
                    else if (userChoice == 9) system.addFriend(currentUser);
                    else if (userChoice == 10) system.deleteAccount(currentUser);
                    else if (userChoice == 11) system.sendMessage(currentUser); // 🔽 ADDED
                    else if (userChoice == 12) system.viewMessages(currentUser); // 🔽 ADDED
                    else if (userChoice == 13) cout << "Logging out...\n\n"; // 🔽 ADDED
                    else cout << "Invalid choice\n\n";

                } while (userChoice != 13 && currentUser != -1);
            }
        }
        else if (choice == 2) system.registerUser();
        else if (choice == 3) {
            cout << "Exiting...\n";
            break;
        }
        else cout << "Invalid choice\n\n";

        cout << "----------------------------------------\n";
    }

    return 0;
}