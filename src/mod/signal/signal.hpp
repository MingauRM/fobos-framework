#include <iostream>
#include <vector>
#include <algorithm>

// Forward declarations
class ScriptSignal;

class ScriptConnection {
    public:
        ScriptConnection(ScriptSignal* signal, int callbackRef)
        : Signal(signal), Callback(callbackRef)
        {}

        bool operator==(const ScriptConnection& other) const
        {
            return Signal == other.Signal && Callback == other.Callback;
        }

        int GetCallback() const
        {
            return Callback;
        }
        
        ScriptSignal* GetSignal() const 
        {
            return Signal;
        }

    private:
        int Callback;
        ScriptSignal* Signal;
};

class ScriptSignal {
    public:
        ScriptSignal();

        void Connect(int ref)
        {
            connections.emplace_back(this, ref);
        }

        void Disconnect(const ScriptConnection& conn)
        {
            auto it = std::find(connections.begin(), connections.end(), conn);
            if (it != connections.end())
            {
                connections.erase(it);
            }
        }

        std::vector<ScriptConnection> GetConnections() const
        {
            return connections;
        }

    private:
        std::vector<ScriptConnection> connections;
};

std::vector<ScriptSignal*> SignalRegistry;

ScriptSignal::ScriptSignal() {
    SignalRegistry.push_back(this);
}