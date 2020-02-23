using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using WebSocketSharp;

public class LobbyManager : MonoBehaviour
{
    [SerializeField] private GameObject watingPanel;
    [SerializeField] private GameObject matchingPanel;
    [SerializeField] private Text playerId;
    [SerializeField] private Text otherId;
    [SerializeField] private Text otherElo;
    [SerializeField] private Text userIdInputFieldText;
    private void Awake()
    {
        _lobbyManager=this;
    }

    public void OpenWatingPanel()
    {
        watingPanel.SetActive(true);
    }

    UserInfo otherUserInfo=new UserInfo();
    public void OpenMatchingPanel(JSONObject data)
    {
        watingPanel.SetActive(false);
        otherUserInfo = JsonUtility.FromJson<UserInfo>(data.ToString());
        Debug.Log("otherid:"+otherUserInfo.userId);
        playerId.text=GetUserId() + "! 그 상대는!";
        otherElo.text=otherUserInfo.elo+"";
        otherId.text=otherUserInfo.userId + "!";
        matchingPanel.SetActive(true);
    }

    public String GetUserId()
    {
        var text=userIdInputFieldText.GetComponent<Text>().text;
        return text;
    }

    private static LobbyManager _lobbyManager;

    public static LobbyManager Instance()
    {
        return _lobbyManager;
    }
}
