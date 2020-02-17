using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LobbyManager : MonoBehaviour
{
    [SerializeField]private GameObject watingPanel;
    private void Awake()
    {
        _lobbyManager=this;
    }

    public void OpenWatingPanel()
    {
        watingPanel.SetActive(true);
    }

    private static LobbyManager _lobbyManager;

    public LobbyManager Instance()
    {
        return _lobbyManager;
    }
}
