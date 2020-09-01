using System.Collections;
using System.Collections.Generic;
using UnityEngine;
public class CameraMover : MonoBehaviour
{
    [SerializeField] private GameObject player;
    void Update()
    {
        Vector3 pos=player.transform.position;
        gameObject.transform.position=new Vector3(pos.x,pos.y,pos.z-10f);
    }
}
