using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UIElements;

public class ObjectMove : MonoBehaviour
{
    private Transform _transform;
    private Vector3 _position;
    void Start()
    {
        _transform=GetComponent<Transform>();
        _position=_transform.position;
    }


    void Update()
    {
       NetworkManager.Instance().SendPos(_position); 
    }
}
