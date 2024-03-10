#include <Scene.h>
#include <algorithm>

bool onKeyPress::SubscribeKeypress(std::function<void(const std::string& id, unsigned long keypress)> evt, std::vector<unsigned long> keys, const std::string& eventId)
{
    try
    {
        if (m_onKeysPressed.find(eventId) != m_onKeysPressed.end())
            m_onKeysPressed[eventId] = evt;
        else
            m_onKeysPressed.insert(std::make_pair(eventId, evt));

        if (m_idVsKeys.find(eventId) == m_idVsKeys.end())
            m_idVsKeys.insert(std::make_pair(eventId, std::vector<unsigned long>()));
        else
            m_idVsKeys[eventId].clear();

        for (unsigned long key : keys)
        {
            m_idVsKeys[eventId].push_back(key);
            if (std::find_if(m_keysToCheck.begin(), m_keysToCheck.end(), [=, &key](unsigned long k) { return k == key; }) == m_keysToCheck.end())
                m_keysToCheck.push_back(key);
        }
        return true;
    }
    catch (const std::exception&) {}
    return false;
}
bool onKeyPress::UnSubscribeKeypress(const std::string& eventId)
{
    try
    {
        auto it = m_onKeysPressed.find(eventId);
        if (it != m_onKeysPressed.end())
            m_onKeysPressed.erase(it);

        auto itkeys = m_idVsKeys.find(eventId);
        if (itkeys != m_idVsKeys.end())
            m_idVsKeys.erase(itkeys);

        updateKeysOfInterest();
        return true;
    }
    catch (const std::exception&) {}
    return false;
}
bool onKeyPress::UnSubscribeAllKeypress(void)
{
    try
    {
        m_onKeysPressed.clear();
        m_idVsKeys.clear();
        m_keysToCheck.clear();
        return true;
    }
    catch (const std::exception&) {}
    return false;
}
bool onKeyPress::OnKeyPress(const std::string& id, unsigned long keypress)
{
    try
    {
        for (auto& evt : m_onKeysPressed)
        {
            if (m_idVsKeys.find(evt.first) != m_idVsKeys.end())
            {
                if (std::find_if(m_idVsKeys[evt.first].begin(), m_idVsKeys[evt.first].end(), [=, &keypress](unsigned long key) {return keypress == key; }) != m_idVsKeys[evt.first].end())
                {
                    if (evt.first.length() > 0)
                        evt.second(evt.first, keypress);
                    else
                        evt.second(id, keypress);
                }
            }
        }
        return true;
    }
    catch (const std::exception&) {}
    return false;
}
void onKeyPress::updateKeysOfInterest(void)
{
    try
    {
        m_keysToCheck.clear();
        std::for_each(m_idVsKeys.begin(), m_idVsKeys.end(), [=](std::pair<const std::string&, std::vector<unsigned long>>p) {
            for (unsigned long &key : p.second)
            {
                if (std::find_if(m_keysToCheck.begin(), m_keysToCheck.end(), [=, &key](unsigned long k) {return key == k; }) == m_keysToCheck.end())
                    m_keysToCheck.push_back(key);
            }
            });
    }
    catch (const std::exception&) { }
}

bool onMouseMove::SubscribeMouse(std::function<void(const std::string& id, float dx, float dy, float start_x, float start_y, MouseButtonStatus button)> evt, const std::string& eventId)
{
    try
    {
        if (m_evOnMouseMove.find(eventId) != m_evOnMouseMove.end())
            m_evOnMouseMove[eventId] = evt;
        else
            m_evOnMouseMove.insert(std::make_pair(eventId, evt));
        return true;
    }
    catch (const std::exception&) {}
    return false;
}
bool onMouseMove::UnSubscribeMouse(const std::string& eventId)
{
    try
    {
        auto it = m_evOnMouseMove.find(eventId);
        if (it != m_evOnMouseMove.end())
            m_evOnMouseMove.erase(it);
        return true;
    }
    catch (const std::exception&) {}
    return false;
}
bool onMouseMove::UnSubscribeAllMouse(void)
{
    try
    {
        m_evOnMouseMove.clear();
        return true;
    }
    catch (const std::exception&) {}
    return false;
}
bool onMouseMove::OnMouseMove(const std::string& id, float xpos, float ypos, MouseButtonStatus btn)
{
    try
    {
        if (m_bfirstMove)
        {
            m_current_x = xpos;
            m_current_y = ypos;
            m_bfirstMove = false;
        }

        float dtx = xpos - m_current_x;
        float dty = ypos - m_current_y;

        for (auto& evt : m_evOnMouseMove)
        {
            if (evt.first.length() > 0)
                evt.second(evt.first, dtx, dty, m_current_x, m_current_y, btn);
            else
                evt.second(id, dtx, dty, m_current_x, m_current_y, btn);
        }
        return true;
    }
    catch (const std::exception&) {  }
    return false;
}

