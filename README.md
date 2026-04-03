#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
using namespace std;

// ==================== CLASS: POST ====================
class Post {
private:
    static int nextPostId;
    int postId;
    string content;
    int authorId;
    string authorName;
    int likes;
    vector<string> comments;
    string privacy;
    int targetUserId;

public:
    Post() : postId(0), authorId(0), likes(0), targetUserId(0) {}

    Post(string content, int authorId, string authorName, string privacy, int targetUserId = 0) {
        this->postId = nextPostId++;
        this->content = content;
        this->authorId = authorId;
        this->authorName = authorName;
        this->privacy = privacy;
        this->targetUserId = targetUserId;
        this->likes = 0;
    }

    bool canView(int viewerId, const vector<int>& friendsList) {
        if (privacy == "public") return true;
        if (privacy == "private" && authorId == viewerId) return true;
        if (privacy == "friends") {
            if (authorId == viewerId) return true;
            for (int friendId : friendsList) {
                if (friendId == authorId) return true;
            }
        }
        if (targetUserId == viewerId) return true;
        return false;
    }

    void addLike() { likes++; }
    void addComment(string comment) { comments.push_back(comment); }

    void display(int viewerId, const vector<int>& friendsList) {
        if (!canView(viewerId, friendsList)) return;

        cout << "\n+--------------------------------------------------+" << endl;
        cout << "| POST #" << postId << endl;
        cout << "| By: " << authorName << endl;
        cout << "| Privacy: " << privacy;
        if (targetUserId != 0) cout << " (Direct to user)";
        cout << endl;
        cout << "| Content: " << content << endl;
        cout << "| [HEART] " << likes << " likes" << endl;
        cout << "| [COMMENTS] (" << comments.size() << "):" << endl;
        for (size_t i = 0; i < comments.size() && i < 3; i++) {
            cout << "|    " << i+1 << ". " << comments[i] << endl;
        }
        cout << "+--------------------------------------------------+" << endl;
    }

    // File saving/loading methods
    string toFileString() {
        stringstream ss;
        ss << postId << "|" << content << "|" << authorId << "|" << authorName << "|"
           << likes << "|" << privacy << "|" << targetUserId << "|";
        for (size_t i = 0; i < comments.size(); i++) {
            ss << comments[i];
            if (i < comments.size() - 1) ss << ",";
        }
        return ss.str();
    }

    void fromFileString(string line) {
        stringstream ss(line);
        string token;

        getline(ss, token, '|'); postId = stoi(token);
        getline(ss, token, '|'); content = token;
        getline(ss, token, '|'); authorId = stoi(token);
        getline(ss, token, '|'); authorName = token;
        getline(ss, token, '|'); likes = stoi(token);
        getline(ss, token, '|'); privacy = token;
        getline(ss, token, '|'); targetUserId = stoi(token);
        getline(ss, token, '|');

        stringstream commentStream(token);
        string comment;
        while (getline(commentStream, comment, ',')) {
            if (!comment.empty()) comments.push_back(comment);
        }

        if (postId >= nextPostId) nextPostId = postId + 1;
    }

    int getPostId() { return postId; }
    int getAuthorId() { return authorId; }
    string getPrivacy() { return privacy; }
};

int Post::nextPostId = 1;

// ==================== CLASS: USER ====================
class User {
private:
    int userId;
    string username;
    string password;
    string email;
    vector<int> friends;
    vector<int> friendRequests;

public:
    User() : userId(0) {}

    User(int id, string name, string pass, string mail) {
        userId = id;
        username = name;
        password = pass;
        email = mail;
    }

    bool checkPassword(string pass) { return password == pass; }

    void addFriend(int friendId) {
        for (int f : friends) {
            if (f == friendId) return;
        }
        friends.push_back(friendId);
    }

    void removeFriend(int friendId) {
        for (size_t i = 0; i < friends.size(); i++) {
            if (friends[i] == friendId) {
                friends.erase(friends.begin() + i);
                break;
            }
        }
    }

    void addFriendRequest(int fromUserId) {
        if (fromUserId == userId) return;
        for (int r : friendRequests) {
            if (r == fromUserId) return;
        }
        friendRequests.push_back(fromUserId);
    }

    void removeFriendRequest(int fromUserId) {
        for (size_t i = 0; i < friendRequests.size(); i++) {
            if (friendRequests[i] == fromUserId) {
                friendRequests.erase(friendRequests.begin() + i);
                break;
            }
        }
    }

    vector<int> getFriendRequests() { return friendRequests; }

    // File saving/loading methods
    string toFileString() {
        stringstream ss;
        ss << userId << "|" << username << "|" << password << "|" << email << "|";
        for (size_t i = 0; i < friends.size(); i++) {
            ss << friends[i];
            if (i < friends.size() - 1) ss << ",";
        }
        ss << "|";
        for (size_t i = 0; i < friendRequests.size(); i++) {
            ss << friendRequests[i];
            if (i < friendRequests.size() - 1) ss << ",";
        }
        return ss.str();
    }

    void fromFileString(string line) {
        stringstream ss(line);
        string token;

        getline(ss, token, '|'); userId = stoi(token);
        getline(ss, token, '|'); username = token;
        getline(ss, token, '|'); password = token;
        getline(ss, token, '|'); email = token;
        getline(ss, token, '|');

        stringstream friendStream(token);
        string friendId;
        while (getline(friendStream, friendId, ',')) {
            if (!friendId.empty()) friends.push_back(stoi(friendId));
        }

        getline(ss, token, '|');
        stringstream requestStream(token);
        string requestId;
        while (getline(requestStream, requestId, ',')) {
            if (!requestId.empty()) friendRequests.push_back(stoi(requestId));
        }
    }

    int getId() { return userId; }
    string getUsername() { return username; }
    string getPassword() { return password; }
    string getEmail() { return email; }
    vector<int> getFriends() { return friends; }

    void displayProfile() {
        cout << "\n+--------------------------------------------------+" << endl;
        cout << "|                 USER PROFILE                    |" << endl;
        cout << "+--------------------------------------------------+" << endl;
        cout << "| ID: " << userId << endl;
        cout << "| Name: @" << username << endl;
        cout << "| Email: " << email << endl;
        cout << "| Friends: " << friends.size() << endl;
        cout << "+--------------------------------------------------+" << endl;
    }
};

// ==================== CLASS: MESSAGE ====================
class Message {
private:
    static int nextMsgId;
    int msgId;
    int fromId;
    string fromName;
    int toId;
    string content;

public:
    Message() : msgId(0), fromId(0), toId(0) {}

    Message(int fromId, string fromName, int toId, string content) {
        this->msgId = nextMsgId++;
        this->fromId = fromId;
        this->fromName = fromName;
        this->toId = toId;
        this->content = content;
    }

    string toFileString() {
        stringstream ss;
        ss << msgId << "|" << fromId << "|" << fromName << "|" << toId << "|" << content;
        return ss.str();
    }

    void fromFileString(string line) {
        stringstream ss(line);
        string token;

        getline(ss, token, '|'); msgId = stoi(token);
        getline(ss, token, '|'); fromId = stoi(token);
        getline(ss, token, '|'); fromName = token;
        getline(ss, token, '|'); toId = stoi(token);
        getline(ss, token, '|'); content = token;

        if (msgId >= nextMsgId) nextMsgId = msgId + 1;
    }

    void display() {
        cout << "[ENVELOPE] From " << fromName << ": " << content << endl;
    }

    int getToId() { return toId; }
};

int Message::nextMsgId = 1;

// ==================== CLASS: SOCIALMEDIA WITH FILE STORAGE ====================
class SocialMediaSystem {
private:
    vector<User> users;
    vector<Post> posts;
    vector<Message> messages;
    int nextUserId;
    User* currentUser;

    // File names
    const string USER_FILE = "users.txt";
    const string POST_FILE = "posts.txt";
    const string MSG_FILE = "messages.txt";

public:
    SocialMediaSystem() {
        nextUserId = 1;
        currentUser = nullptr;
        loadAllData();

        // If no users exist, create demo users
        if (users.empty()) {
            cout << "\n[SYSTEM] Creating demo users...\n" << endl;
            registerUser("alice", "1234", "alice@social.com");
            registerUser("bob", "1234", "bob@social.com");
            registerUser("charlie", "1234", "charlie@social.com");

            // Add demo friendship
            User* alice = findUserByUsername("alice");
            User* bob = findUserByUsername("bob");
            if (alice && bob) {
                addFriendship(alice->getId(), bob->getId());
            }

            // Add demo posts
            if (alice) {
                createPost(alice->getId(), "Hello everyone! This is my first public post!", "public");
                createPost(alice->getId(), "Only my friends can see this :)", "friends");
            }
            if (bob) {
                createPost(bob->getId(), "Having a great day! #coding", "public");
            }

            saveAllData();
        }
    }

    ~SocialMediaSystem() {
        saveAllData();
    }

    // ========== FILE HANDLING FUNCTIONS ==========

    void saveUsers() {
        ofstream file(USER_FILE);
        for (User& user : users) {
            file << user.toFileString() << endl;
        }
        file.close();
        cout << "[SAVED] Users to file" << endl;
    }

    void loadUsers() {
        ifstream file(USER_FILE);
        if (!file.is_open()) return;

        users.clear();
        string line;
        int maxId = 0;

        while (getline(file, line)) {
            User u;
            u.fromFileString(line);
            users.push_back(u);
            if (u.getId() > maxId) maxId = u.getId();
        }

        nextUserId = maxId + 1;
        file.close();
        cout << "[LOADED] " << users.size() << " users" << endl;
    }

    void savePosts() {
        ofstream file(POST_FILE);
        for (Post& post : posts) {
            file << post.toFileString() << endl;
        }
        file.close();
        cout << "[SAVED] Posts to file" << endl;
    }

    void loadPosts() {
        ifstream file(POST_FILE);
        if (!file.is_open()) return;

        posts.clear();
        string line;

        while (getline(file, line)) {
            Post p;
            p.fromFileString(line);
            posts.push_back(p);
        }

        file.close();
        cout << "[LOADED] " << posts.size() << " posts" << endl;
    }

    void saveMessages() {
        ofstream file(MSG_FILE);
        for (Message& msg : messages) {
            file << msg.toFileString() << endl;
        }
        file.close();
        cout << "[SAVED] Messages to file" << endl;
    }

    void loadMessages() {
        ifstream file(MSG_FILE);
        if (!file.is_open()) return;

        messages.clear();
        string line;

        while (getline(file, line)) {
            Message m;
            m.fromFileString(line);
            messages.push_back(m);
        }

        file.close();
        cout << "[LOADED] " << messages.size() << " messages" << endl;
    }

    void saveAllData() {
        saveUsers();
        savePosts();
        saveMessages();
        cout << "\n[SYSTEM] All data saved!\n" << endl;
    }

    void loadAllData() {
        cout << "\n[SYSTEM] Loading data..." << endl;
        loadUsers();
        loadPosts();
        loadMessages();
    }

    // ========== USER FUNCTIONS ==========

    bool registerUser(string username, string password, string email) {
        for (User& user : users) {
            if (user.getUsername() == username) {
                cout << "[ERROR] Username already exists!" << endl;
                return false;
            }
        }

        User newUser(nextUserId++, username, password, email);
        users.push_back(newUser);
        saveUsers();
        cout << "[SUCCESS] Registration successful! Welcome " << username << "!" << endl;
        return true;
    }

    bool login(string username, string password) {
        for (User& user : users) {
            if (user.getUsername() == username && user.checkPassword(password)) {
                currentUser = &user;
                cout << "\n[SUCCESS] Login successful! Welcome back, @" << username << "!\n" << endl;
                return true;
            }
        }
        cout << "[ERROR] Invalid username or password!" << endl;
        return false;
    }

    void logout() {
        currentUser = nullptr;
        cout << "[SUCCESS] Logged out successfully!" << endl;
    }

    User* findUserByUsername(string username) {
        for (User& user : users) {
            if (user.getUsername() == username) {
                return &user;
            }
        }
        return nullptr;
    }

    User* findUserById(int id) {
        for (User& user : users) {
            if (user.getId() == id) {
                return &user;
            }
        }
        return nullptr;
    }

    void addFriendship(int userId1, int userId2) {
        User* u1 = findUserById(userId1);
        User* u2 = findUserById(userId2);
        if (u1 && u2) {
            u1->addFriend(userId2);
            u2->addFriend(userId1);
            u1->removeFriendRequest(userId2);
            u2->removeFriendRequest(userId1);
            saveUsers();
        }
    }

    bool sendFriendRequest(int fromUserId, int toUserId) {
        User* from = findUserById(fromUserId);
        User* to = findUserById(toUserId);
        if (!from || !to) return false;
        if (fromUserId == toUserId) return false;

        // check already friends
        vector<int> friends = from->getFriends();
        for (int f : friends) if (f == toUserId) return false;

        // check existing request
        vector<int> toRequests = to->getFriendRequests();
        for (int r : toRequests) if (r == fromUserId) return false;

        to->addFriendRequest(fromUserId);
        saveUsers();
        return true;
    }

    vector<int> getPendingFriendRequests(int userId) {
        User* u = findUserById(userId);
        if (!u) return {};
        return u->getFriendRequests();
    }

    void acceptFriendRequest(int currentUserId, int requesterId) {
        User* me = findUserById(currentUserId);
        User* requester = findUserById(requesterId);
        if (!me || !requester) return;

        // request must exist
        vector<int> reqs = me->getFriendRequests();
        bool found = false;
        for (int r : reqs) if (r == requesterId) found = true;
        if (!found) return;

        addFriendship(currentUserId, requesterId);
    }

    void declineFriendRequest(int currentUserId, int requesterId) {
        User* me = findUserById(currentUserId);
        if (!me) return;
        me->removeFriendRequest(requesterId);
        saveUsers();
    }

    // ========== POST FUNCTIONS ==========

    void createPost(int authorId, string content, string privacy, int targetUserId = 0) {
        User* author = findUserById(authorId);
        if (!author) return;

        Post newPost(content, authorId, author->getUsername(), privacy, targetUserId);
        posts.push_back(newPost);
        savePosts();
        cout << "\n[SUCCESS] Post created! (Privacy: " << privacy << ")\n" << endl;
    }

    vector<Post> getUserPosts(int userId) {
        vector<Post> userPosts;
        for (Post& post : posts) {
            if (post.getAuthorId() == userId) {
                userPosts.push_back(post);
            }
        }
        return userPosts;
    }

    // ========== MESSAGE FUNCTIONS ==========

    void sendMessage(int fromId, int toId, string content) {
        User* from = findUserById(fromId);
        User* to = findUserById(toId);
        if (!from || !to) {
            cout << "[ERROR] User not found!" << endl;
            return;
        }

        Message newMsg(fromId, from->getUsername(), toId, content);
        messages.push_back(newMsg);
        saveMessages();
        cout << "[SUCCESS] Message sent to " << to->getUsername() << "!" << endl;
    }

    vector<Message> getUserMessages(int userId) {
        vector<Message> userMessages;
        for (Message& msg : messages) {
            if (msg.getToId() == userId) {
                userMessages.push_back(msg);
            }
        }
        return userMessages;
    }

    // ========== DISPLAY FUNCTIONS ==========

    void displayNewsFeed() {
        if (!currentUser) return;

        cout << "\n+--------------------------------------------------+" << endl;
        cout << "|                 NEWS FEED                       |" << endl;
        cout << "+--------------------------------------------------+" << endl;

        vector<Post> visiblePosts;
        for (Post& post : posts) {
            if (post.canView(currentUser->getId(), currentUser->getFriends())) {
                visiblePosts.push_back(post);
            }
        }

        reverse(visiblePosts.begin(), visiblePosts.end());

        if (visiblePosts.empty()) {
            cout << "| [EMPTY] No posts to show. Add friends!          |" << endl;
            cout << "+--------------------------------------------------+" << endl;
            return;
        }

        for (Post& post : visiblePosts) {
            post.display(currentUser->getId(), currentUser->getFriends());
        }
    }

    void displayMyPosts() {
        if (!currentUser) return;

        cout << "\n+--------------------------------------------------+" << endl;
        cout << "|              YOUR POSTS                         |" << endl;
        cout << "+--------------------------------------------------+" << endl;

        vector<Post> myPosts = getUserPosts(currentUser->getId());

        if (myPosts.empty()) {
            cout << "| [EMPTY] No posts yet. Create your first post!    |" << endl;
            cout << "+--------------------------------------------------+" << endl;
            return;
        }

        for (Post& post : myPosts) {
            post.display(currentUser->getId(), currentUser->getFriends());
        }
    }

    void displayMyMessages() {
        if (!currentUser) return;

        cout << "\n+--------------------------------------------------+" << endl;
        cout << "|               YOUR MESSAGES                     |" << endl;
        cout << "+--------------------------------------------------+" << endl;

        vector<Message> myMessages = getUserMessages(currentUser->getId());

        if (myMessages.empty()) {
            cout << "| [EMPTY] No messages yet.                       |" << endl;
            cout << "+--------------------------------------------------+" << endl;
            return;
        }

        for (Message& msg : myMessages) {
            cout << "| ";
            msg.display();
        }
        cout << "+--------------------------------------------------+" << endl;
    }

    void listAllUsers() {
        cout << "\n+--------------------------------------------------+" << endl;
        cout << "|                 ALL USERS                       |" << endl;
        cout << "+--------------------------------------------------+" << endl;

        for (User& user : users) {
            if (currentUser && user.getId() != currentUser->getId()) {
                cout << "|   [USER] @" << user.getUsername() << " (ID: " << user.getId() << ")" << endl;
            }
        }
        cout << "+--------------------------------------------------+" << endl;
    }

    bool isLoggedIn() { return currentUser != nullptr; }
    User* getCurrentUser() { return currentUser; }
};

// ==================== MAIN FUNCTION ====================
int main() {
    SocialMediaSystem socialMedia;
    int choice;

    cout << "\n==================================================" << endl;
    cout << "         WELCOME TO SOCIAL MEDIA SYSTEM          " << endl;
    cout << "         Connect with friends!                   " << endl;
    cout << "==================================================" << endl;

    while (true) {
        if (!socialMedia.isLoggedIn()) {
            cout << "\n+--------------------------------------------------+" << endl;
            cout << "|                   MAIN MENU                     |" << endl;
            cout << "+--------------------------------------------------+" << endl;
            cout << "| 1. Login                                        |" << endl;
            cout << "| 2. Register                                     |" << endl;
            cout << "| 0. Exit                                         |" << endl;
            cout << "+--------------------------------------------------+" << endl;
            cout << "Choose: ";
            cin >> choice;

            if (choice == 1) {
                string username, password;
                cout << "Username: ";
                cin >> username;
                cout << "Password: ";
                cin >> password;
                socialMedia.login(username, password);
            }
            else if (choice == 2) {
                string username, password, email;
                cout << "Username: ";
                cin >> username;
                cout << "Password: ";
                cin >> password;
                cout << "Email: ";
                cin >> email;
                socialMedia.registerUser(username, password, email);
            }
            else if (choice == 0) {
                cout << "\n[GOODBYE] Thanks for using Social Media System!" << endl;
                cout << "[SYSTEM] Saving data..." << endl;
                break;
            }
        }
        else {
            User* current = socialMedia.getCurrentUser();

            cout << "\n+--------------------------------------------------+" << endl;
            cout << "|             SOCIAL MEDIA DASHBOARD               |" << endl;
            cout << "+--------------------------------------------------+" << endl;
            cout << "|  Logged in as: @" << current->getUsername() << endl;
            cout << "+--------------------------------------------------+" << endl;
            cout << "|  1. View News Feed                              |" << endl;
            cout << "|  2. Create a Post                               |" << endl;
            cout << "|  3. View My Posts                               |" << endl;
            cout << "|  4. View Profile                                |" << endl;
            cout << "|  5. Send Friend Request                         |" << endl;
            cout << "|  6. List All Users                              |" << endl;
            cout << "|  7. View Friend Requests                        |" << endl;
            cout << "|  8. Send Message                                |" << endl;
            cout << "|  9. View Messages                               |" << endl;
            cout << "| 10. View Someone's Profile                      |" << endl;
            cout << "| 11. Post to Someone's Wall                      |" << endl;
            cout << "+--------------------------------------------------+" << endl;
            cout << "|  0. Logout                                      |" << endl;
            cout << "+--------------------------------------------------+" << endl;
            cout << "Choice: ";
            cin >> choice;

            switch(choice) {
                case 1:
                    socialMedia.displayNewsFeed();
                    break;

                case 2: {
                    cin.ignore();
                    string content, privacy;
                    cout << "Enter your post: ";
                    getline(cin, content);
                    cout << "Privacy (public/friends/private): ";
                    cin >> privacy;

                    if (privacy != "public" && privacy != "friends" && privacy != "private") {
                        cout << "[WARNING] Invalid privacy! Setting to public." << endl;
                        privacy = "public";
                    }

                    socialMedia.createPost(current->getId(), content, privacy);
                    break;
                }

                case 3:
                    socialMedia.displayMyPosts();
                    break;

                case 4:
                    current->displayProfile();
                    break;

                case 5: {
                    string friendName;
                    cout << "Enter friend's username: ";
                    cin >> friendName;
                    User* friendUser = socialMedia.findUserByUsername(friendName);
                    if (friendUser && friendUser->getId() != current->getId()) {
                        if (socialMedia.sendFriendRequest(current->getId(), friendUser->getId())) {
                            cout << "[SUCCESS] Friend request sent to " << friendName << "!" << endl;
                        } else {
                            cout << "[INFO] Already friends or request pending." << endl;
                        }
                    } else {
                        cout << "[ERROR] User not found or can't request yourself!" << endl;
                    }
                    break;
                }

                case 6:
                    socialMedia.listAllUsers();
                    break;

                case 7: {
                    string toUsername, message;
                    cout << "Send message to: ";
                    cin >> toUsername;
                    User* recipient = socialMedia.findUserByUsername(toUsername);
                    if (recipient && recipient->getId() != current->getId()) {
                        cin.ignore();
                        cout << "Your message: ";
                        getline(cin, message);
                        socialMedia.sendMessage(current->getId(), recipient->getId(), message);
                    } else {
                        cout << "[ERROR] User not found!" << endl;
                    }
                    break;
                }

                case 8:
                    socialMedia.displayMyMessages();
                    break;

                case 9: {
                    string username;
                    cout << "Enter username to view: ";
                    cin >> username;
                    User* target = socialMedia.findUserByUsername(username);
                    if (target) {
                        target->displayProfile();
                    } else {
                        cout << "[ERROR] User not found!" << endl;
                    }
                    break;
                }

                case 10: {
                    cin.ignore();
                    string toUsername, content;
                    cout << "Post to whose wall? (username): ";
                    getline(cin, toUsername);
                    User* target = socialMedia.findUserByUsername(toUsername);
                    if (target && target->getId() != current->getId()) {
                        cout << "Enter your post: ";
                        getline(cin, content);
                        socialMedia.createPost(current->getId(), content, "private", target->getId());
                        cout << "[SUCCESS] Posted to " << target->getUsername() << "'s wall!" << endl;
                    } else {
                        cout << "[ERROR] User not found or can't post to yourself!" << endl;
                    }
                    break;
                }

                case 0:
                    socialMedia.logout();
                    break;

                default:
                    cout << "[ERROR] Invalid choice!" << endl;
            }
        }
    }

    return 0;
}
# SocialMedia
