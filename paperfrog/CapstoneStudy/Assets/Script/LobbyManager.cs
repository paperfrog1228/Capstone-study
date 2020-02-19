using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using WebSocketSharp;

public class LobbyManager : MonoBehaviour
{
    [SerializeField]private GameObject watingPanel;
    [SerializeField] private Text userIdInputFieldText;
    private void Awake()
    {
        _lobbyManager=this;
    }

    public void OpenWatingPanel()
    {
        watingPanel.SetActive(true);
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
