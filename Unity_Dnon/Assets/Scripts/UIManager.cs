using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UIManager : MonoBehaviour
{
    [SerializeField] private GameObject MapPanel;
    public void OpenMap() {
        MapPanel.SetActive(true);
    }
}
